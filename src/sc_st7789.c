#include "sc_st7789.h"
#include "sc_color.h"
#include "sc_painter.h"
#include <stdarg.h>
#include <string.h>

int st7789_draw_point(struct st7789_screen *self, struct point p, unsigned long color);
int st7789_size(struct st7789_screen *self, struct point *p);
int st7789_fill(struct st7789_screen *self, struct point p1, struct point p2, unsigned long color);

static struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn_t)st7789_draw_point,
	.size = (drawing_size_fn_t)st7789_size,
	.fill = (drawing_fill_fn_t)st7789_fill,
};

static int st7789_write(struct st7789_screen *self, int cmd, int arg_cnt, ...)
{
	va_list args;
	int i;

	va_start(args, arg_cnt);

	if (sc_adaptor_write_cmd(self->adaptor, cmd))
		return 1;
	for (i = 0; i < arg_cnt; i++) {
		if (sc_adaptor_write_data(self->adaptor, va_arg(args, int)))
			return 2;
	}

	va_end(args);
	return 0;
}

int st7789_set_address(struct st7789_screen *self, struct point p1, struct point p2)
{
	/// column address settings
	if (st7789_write(self, 0x2A, 4, p1.x >> 16, p1.x & 0xFF, p2.x >> 16, p2.x & 0xFF))
		return 1;
	/// row address setting
	if (st7789_write(self, 0x2B, 4, p1.y >> 16, p1.y & 0xFF, p2.y >> 16, p2.y & 0xFF))
		return 2;

	return 0;
}

int st7789_draw_point(struct st7789_screen *self, struct point p, unsigned long color)
{
	unsigned long fixed_color;

	if (p.x >= self->size.x || p.y >= self->size.y)
		return 1;

	if (sc_adaptor_start_transmit(self->adaptor))
		return 0xFE;

	if (st7789_set_address(self, p, p))
		return 2;

	fixed_color = color_to_16bit(color);

	/// memory write
	if (st7789_write(self, 0x2C, 2, fixed_color >> 16, fixed_color & 0xFF))
		return 3;

	if (sc_adaptor_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int st7789_size(struct st7789_screen *self, struct point *p)
{
	return point_init(p, self->size.x, self->size.y);
}

int st7789_fill(struct st7789_screen *self, struct point p1, struct point p2, unsigned long color)
{
	unsigned long fixed_color;
	int n = ABS((p1.x - p2.x) * (p1.y - p2.y));

	if (sc_adaptor_start_transmit(self->adaptor))
		return 0xFE;

	if (st7789_set_address(self, p1, p2))
		return 1;
	if (st7789_write(self, 0x2C, 0))
		return 2;

	fixed_color = color_to_16bit(color);

	while (n--) {
		if (sc_adaptor_write_data(self->adaptor, fixed_color >> 16))
			return 3;
		if (sc_adaptor_write_data(self->adaptor, fixed_color & 0xFF))
			return 3;
	}

	if (sc_adaptor_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int st7789_prepare(struct st7789_screen *self)
{
	if (sc_adaptor_start_transmit(self->adaptor))
		return 0xFE;

	/// Memory Data Access Control
	if (st7789_write(self, 0x36, 1, 0x00))
		return 1;
	/// RGB565 (16-bit color)
	if (st7789_write(self, 0x3A, 1, 0x05))
		return 2;

	/// Porch Setting
	if (st7789_write(self, 0xB2, 5, 0x0C, 0x0C, 0x00, 0x33, 0x33))
		return 3;

	/// Gate Control
	if (st7789_write(self, 0xB7, 1, 0x00))
		return 4;

	/// VCOM Setting
	if (st7789_write(self, 0xBB, 1, 0x3F))
		return 5;

	/// LCM Control
	if (st7789_write(self, 0xC0, 1, 0x2C))
		return 6;

	/// VDV and VRH Command Enable
	if (st7789_write(self, 0xC2, 1, 0x01))
		return 7;

	/// VRH Set
	if (st7789_write(self, 0xC3, 1, 0x0D))
		return 8;

	/// VDV Set
	if (st7789_write(self, 0xC4, 1, 0x20))
		return 9;

	/// Frame Rate Control in Normal Mode
	if (st7789_write(self, 0xC6, 1, 0x03))
		return 10;

	/// Power Control 1
	if (st7789_write(self, 0xD0, 2, 0xA4, 0xA1))
		return 11;

	/// Positive Voltage Gamma Control
	if (st7789_write(self, 0xE0, 14, 0xF0, 0x03, 0x09, 0x03, 0x03, 0x10, 0x2D, 0x43, 0x3F, 0x33, 0x0D, 0x0E, 0x29, 0x32))
		return 12;

	/// Negative Voltage Gamma Control
	if (st7789_write(self, 0xE1, 14, 0xF0, 0x0C, 0x10, 0x0E, 0x0E, 0x0A, 0x2D, 0x33, 0x45, 0x3A, 0x14, 0x19, 0x31, 0x37))
		return 13;

	/// Display Inversion On
	if (st7789_write(self, 0x21, 0))
		return 15;

	/// Sleep Out
	if (st7789_write(self, 0x11, 0))
		return 16;

	/// wait for power stability
	delay(100);

	/// Display on
	if (st7789_write(self, 0x29, 0))
		return 17;

	if (sc_adaptor_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int st7789_init(struct st7789_screen *self, struct sc_adaptor_i **adaptor)
{
	memset(self, 0, sizeof(struct st7789_screen));

	self->drawing_board = &drawing_interface;
	self->adaptor = adaptor;

	self->size.x = 240;
	self->size.y = 240;

	if (st7789_prepare(self))
		return 1;

	return 0;
}
