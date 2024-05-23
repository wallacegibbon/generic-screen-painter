#include "sc_st7735.h"
#include "sc_color.h"
#include "sc_painter.h"
#include <stdarg.h>
#include <string.h>

int st7735_draw_point(struct st7735_screen *self, struct point p, unsigned long color);
int st7735_size(struct st7735_screen *self, struct point *p);
int st7735_fill(struct st7735_screen *self, struct point p1, struct point p2, unsigned long color);

static struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn_t)st7735_draw_point,
	.size = (drawing_size_fn_t)st7735_size,
	.fill = (drawing_fill_fn_t)st7735_fill,
};

static inline int st7735_write_data(struct st7735_screen *self, int data)
{
	return (*self->adaptor)->write_data(self->adaptor, data);
}

static inline int st7735_write_cmd(struct st7735_screen *self, int data)
{
	return (*self->adaptor)->write_cmd(self->adaptor, data);
}

static int st7735_write(struct st7735_screen *self, int cmd, int arg_cnt, ...)
{
	va_list args;
	int i;

	va_start(args, arg_cnt);

	if (st7735_write_cmd(self, cmd))
		return 1;
	for (i = 0; i < arg_cnt; i++) {
		if (st7735_write_data(self, va_arg(args, int)))
			return 2;
	}

	va_end(args);
	return 0;
}

int st7735_set_address(struct st7735_screen *self, struct point p1, struct point p2)
{
	int t1, t2;

	t1 = p1.x + 1;
	t2 = p2.x + 1;
	/// column address settings
	if (st7735_write(self, 0x2A, 4, t1 >> 16, t1 & 0xFF, t2 >> 16, t2 & 0xFF))
		return 1;

	t1 = p1.y + 26;
	t2 = p2.y + 26;
	/// row address setting
	if (st7735_write(self, 0x2B, 4, t1 >> 16, t1 & 0xFF, t2 >> 16, t2 & 0xFF))
		return 2;

	return 0;
}

int st7735_draw_point(struct st7735_screen *self, struct point p, unsigned long color)
{
	unsigned long fixed_color;

	if (p.x >= self->size.x || p.y >= self->size.y)
		return 1;
	if (st7735_set_address(self, p, p))
		return 2;

	fixed_color = color_to_16bit(color);

	/// memory write
	if (st7735_write(self, 0x2C, 2, fixed_color >> 16, fixed_color & 0xFF))
		return 3;

	return 0;
}

int st7735_size(struct st7735_screen *self, struct point *p)
{
	return point_init(p, self->size.x, self->size.y);
}

int st7735_fill(struct st7735_screen *self, struct point p1, struct point p2, unsigned long color)
{
	unsigned long fixed_color;
	int n = ABS((p1.x - p2.x) * (p1.y - p2.y));

	if (st7735_set_address(self, p1, p2))
		return 1;
	if (st7735_write(self, 0x2C, 0))
		return 2;

	fixed_color = color_to_16bit(color);

	while (n--) {
		if (st7735_write_data(self, fixed_color >> 16))
			return 3;
		if (st7735_write_data(self, fixed_color & 0xFF))
			return 3;
	}

	return 0;
}

int st7735_prepare(struct st7735_screen *self)
{
	/// Memory Data Access Control
	if (st7735_write(self, 0x36, 1, 0x78))
		return 1;
	/// RGB565 (16-bit color)
	if (st7735_write(self, 0x3A, 1, 0x05))
		return 2;

	/// Set the frame frequency of the full colors normal mode
	/// Frame rate = fosc / ((RTNA x 2 + 40) x (LINE + FPA + BPA + 2))
	/// fosc = 850kHz
	if (st7735_write(self, 0xB1, 3, 0x05, 0x3A, 0x3A))
		return 3;

	/// Set the frame frequency of the Idle mode
	/// Frame rate = fosc / ((RTNB x 2 + 40) x (LINE + FPB + BPB + 2))
	/// fosc = 850kHz
	if (st7735_write(self, 0xB2, 3, 0x05, 0x3A, 0x3A))
		return 4;

	/// Set the frame frequency of the Partial mode/ full colors
	if (st7735_write(self, 0xB3, 6, 0x05, 0x3A, 0x3A, 0x05, 0x3A, 0x3A))
		return 5;

	if (st7735_write(self, 0xB4, 1, 0x03))
		return 6;
	if (st7735_write(self, 0xC0, 3, 0x62, 0x02, 0x04))
		return 7;
	if (st7735_write(self, 0xC1, 1, 0xC0))
		return 8;
	if (st7735_write(self, 0xC2, 2, 0x0D, 0x00))
		return 9;
	if (st7735_write(self, 0xC3, 2, 0x8D, 0x6A))
		return 10;
	if (st7735_write(self, 0xC4, 2, 0x8D, 0xEE))
		return 11;
	/// VCOM
	if (st7735_write(self, 0xC5, 1, 0x0E))
		return 12;

	/// Gamma (‘+’polarity) Correction Characteristics Setting
	if (st7735_write(self, 0xE0, 16, 0x10, 0x0E, 0x02, 0x03, 0x0E, 0x07, 0x02, 0x07, 0x0A, 0x12, 0x27, 0x37, 0x00, 0x0D, 0x0E, 0x10))
		return 13;

	/// Gamma ‘-’polarity Correction Characteristics Setting
	if (st7735_write(self, 0xE1, 16, 0x10, 0x0E, 0x03, 0x03, 0x0F, 0x06, 0x02, 0x08, 0x0A, 0x13, 0x26, 0x36, 0x00, 0x0D, 0x0E, 0x10))
		return 14;

	/// Display Inversion On
	if (st7735_write(self, 0x21, 0))
		return 15;

	/// Sleep Out
	if (st7735_write(self, 0x11, 0))
		return 16;

	/// wait for power stability
	delay(100);

	/// Display on
	if (st7735_write(self, 0x29, 0))
		return 17;

	return 0;
}

int st7735_init(struct st7735_screen *self, struct st7735_adaptor_i **adaptor)
{
	memset(self, 0, sizeof(struct st7735_screen));

	self->drawing_board = &drawing_interface;
	self->adaptor = adaptor;

	self->size.x = 160;
	self->size.y = 80;

	if (st7735_prepare(self))
		return 1;

	return 0;
}
