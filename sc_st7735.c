#include "sc_st7735.h"
#include "sc_color.h"
#include "sc_painter.h"
#include <string.h>

void st7735_draw_point(struct st7735_screen *self, struct point p, uint32_t color);
void st7735_size(struct st7735_screen *self, struct point *p);
void st7735_fill(struct st7735_screen *self, struct point p1, struct point p2, uint32_t color);

static struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn_t)st7735_draw_point,
	.size = (drawing_size_fn_t)st7735_size,
	.fill = (drawing_fill_fn_t)st7735_fill,
};

static inline void st7735_write_data_16(struct st7735_screen *self, uint16_t data) {
	(*self->adaptor)->write_data_16(self->adaptor, data);
}

static inline void st7735_write_data(struct st7735_screen *self, uint8_t data) {
	(*self->adaptor)->write_data(self->adaptor, data);
}

static inline void st7735_write_cmd(struct st7735_screen *self, uint8_t data) {
	(*self->adaptor)->write_cmd(self->adaptor, data);
}

void st7735_set_address(struct st7735_screen *self, struct point p1, struct point p2) {
	/// column address settings
	st7735_write_cmd(self, 0x2A);
	st7735_write_data_16(self, p1.x + 1);
	st7735_write_data_16(self, p2.x + 1);

	/// row address setting
	st7735_write_cmd(self, 0x2B);
	st7735_write_data_16(self, p1.y + 26);
	st7735_write_data_16(self, p2.y + 26);

	/// memory write
	st7735_write_cmd(self, 0x2C);
}

void st7735_draw_point(struct st7735_screen *self, struct point p, uint32_t color) {
	if (p.x >= self->size.x || p.y >= self->size.y)
		return;

	st7735_set_address(self, p, p);
	st7735_write_data_16(self, color_to_16bit(color));
}

void st7735_size(struct st7735_screen *self, struct point *p) {
	point_init(p, self->size.x, self->size.y);
}

/// The default `fill` calls `draw_point`, which will cause many
/// unnecessary `set_address` invocations.
void st7735_fill(struct st7735_screen *self, struct point p1, struct point p2, uint32_t color) {
	int n = ABS((p1.x - p2.x) * (p1.y - p2.y));

	st7735_set_address(self, p1, p2);
	while (n--)
		st7735_write_data_16(self, color_to_16bit(color));
}

void st7735_prepare(struct st7735_screen *self) {
	st7735_write_cmd(self, 0x11);
	delay(100);

	/// display inversion mode (0 is black, -1 is white)
	st7735_write_cmd(self, 0x21);

	/// Set the frame frequency of the full colors normal mode
	st7735_write_cmd(self, 0xB1);
	/// Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
	/// fosc = 850kHz
	st7735_write_data(self, 0x05); // RTNA
	st7735_write_data(self, 0x3A); // FPA
	st7735_write_data(self, 0x3A); // BPA

	/// Set the frame frequency of the Idle mode
	st7735_write_cmd(self, 0xB2);
	/// Frame rate=fosc/((RTNB x 2 + 40) x (LINE + FPB + BPB +2))
	/// fosc = 850kHz
	st7735_write_data(self, 0x05); // RTNB
	st7735_write_data(self, 0x3A); // FPB
	st7735_write_data(self, 0x3A); // BPB

	/// Set the frame frequency of the Partial mode/ full colors
	st7735_write_cmd(self, 0xB3);
	st7735_write_data(self, 0x05);
	st7735_write_data(self, 0x3A);
	st7735_write_data(self, 0x3A);
	st7735_write_data(self, 0x05);
	st7735_write_data(self, 0x3A);
	st7735_write_data(self, 0x3A);

	st7735_write_cmd(self, 0xB4);
	st7735_write_data(self, 0x03);

	st7735_write_cmd(self, 0xC0);
	st7735_write_data(self, 0x62);
	st7735_write_data(self, 0x02);
	st7735_write_data(self, 0x04);

	st7735_write_cmd(self, 0xC1);
	st7735_write_data(self, 0xC0);

	st7735_write_cmd(self, 0xC2);
	st7735_write_data(self, 0x0D);
	st7735_write_data(self, 0x00);

	st7735_write_cmd(self, 0xC3);
	st7735_write_data(self, 0x8D);
	st7735_write_data(self, 0x6A);

	st7735_write_cmd(self, 0xC4);
	st7735_write_data(self, 0x8D);
	st7735_write_data(self, 0xEE);

	st7735_write_cmd(self, 0xC5);
	st7735_write_data(self, 0x0E); /// VCOM

	st7735_write_cmd(self, 0xE0);
	st7735_write_data(self, 0x10);
	st7735_write_data(self, 0x0E);
	st7735_write_data(self, 0x02);
	st7735_write_data(self, 0x03);
	st7735_write_data(self, 0x0E);
	st7735_write_data(self, 0x07);
	st7735_write_data(self, 0x02);
	st7735_write_data(self, 0x07);
	st7735_write_data(self, 0x0A);
	st7735_write_data(self, 0x12);
	st7735_write_data(self, 0x27);
	st7735_write_data(self, 0x37);
	st7735_write_data(self, 0x00);
	st7735_write_data(self, 0x0D);
	st7735_write_data(self, 0x0E);
	st7735_write_data(self, 0x10);

	st7735_write_cmd(self, 0xE1);
	st7735_write_data(self, 0x10);
	st7735_write_data(self, 0x0E);
	st7735_write_data(self, 0x03);
	st7735_write_data(self, 0x03);
	st7735_write_data(self, 0x0F);
	st7735_write_data(self, 0x06);
	st7735_write_data(self, 0x02);
	st7735_write_data(self, 0x08);
	st7735_write_data(self, 0x0A);
	st7735_write_data(self, 0x13);
	st7735_write_data(self, 0x26);
	st7735_write_data(self, 0x36);
	st7735_write_data(self, 0x00);
	st7735_write_data(self, 0x0D);
	st7735_write_data(self, 0x0E);
	st7735_write_data(self, 0x10);

	/// 16 bit color
	st7735_write_cmd(self, 0x3A);
	st7735_write_data(self, 0x05);

	st7735_write_cmd(self, 0x36);
	st7735_write_data(self, 0x78);

	/// display on
	st7735_write_cmd(self, 0x29);
}

void st7735_init(struct st7735_screen *self, struct st7735_adaptor_i **adaptor) {
	memset(self, 0, sizeof(struct st7735_screen));

	self->drawing_board = &drawing_interface;
	self->adaptor = adaptor;

	self->size.x = 160;
	self->size.y = 80;

	st7735_prepare(self);
}
