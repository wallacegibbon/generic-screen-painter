#include "sc_st7789.h"
#include "sc_point_iterator.h"
#include <stddef.h>
#include <string.h>

void st7789_draw_point(struct ST7789_Screen *self, struct Point p, int color);
void st7789_size(struct ST7789_Screen *self, struct Point *p);
void st7789_fill(struct ST7789_Screen *self, struct Point p1, struct Point p2, int color);

static const struct DrawingBoardInterface drawing_board_vtable = {
	.draw_point = (DrawingBoardDrawPoint)st7789_draw_point,
	.size = (DrawingBoardSize)st7789_size,
	.fill = (DrawingBoardFill)st7789_fill,
};

static inline void st7789_write_data_16(struct ST7789_Screen *self, uint16_t data) {
	(*self->adaptor)->write_data_16(self->adaptor, data);
}

static inline void st7789_write_data(struct ST7789_Screen *self, uint8_t data) {
	(*self->adaptor)->write_data(self->adaptor, data);
}

static inline void st7789_write_cmd(struct ST7789_Screen *self, uint8_t data) {
	(*self->adaptor)->write_cmd(self->adaptor, data);
}

void st7789_set_address(struct ST7789_Screen *self, struct Point p1, struct Point p2) {
	/// column address settings
	st7789_write_cmd(self, 0x2A);
	st7789_write_data_16(self, p1.x);
	st7789_write_data_16(self, p2.x);

	/// row address setting
	st7789_write_cmd(self, 0x2B);
	st7789_write_data_16(self, p1.y);
	st7789_write_data_16(self, p2.y);

	/// memory write
	st7789_write_cmd(self, 0x2C);
}

void st7789_draw_point(struct ST7789_Screen *self, struct Point p, int color) {
	if (p.x >= self->size.x || p.y >= self->size.y) return;

	st7789_set_address(self, p, p);
	st7789_write_data_16(self, (uint16_t)color);
}

void st7789_size(struct ST7789_Screen *self, struct Point *p) {
	point_initialize(p, self->size.x, self->size.y);
}

/// The default `fill` calls `draw_point`, which will cause many
/// unnecessary `set_address` invocations.
void st7789_fill(struct ST7789_Screen *self, struct Point p1, struct Point p2, int color) {
	int n = ABS((p1.x - p2.x) * (p1.y - p2.y));

	st7789_set_address(self, p1, p2);
	while (n--)
		st7789_write_data_16(self, (uint16_t)color);
}

void st7789_prepare(struct ST7789_Screen *self) {
	/// Memory Data Access Control
	st7789_write_cmd(self, 0x36);
	st7789_write_data(self, 0x00);

	/// RGB 5-6-5-bit
	st7789_write_cmd(self, 0x3A);
	st7789_write_data(self, 0x05);

	/// Porch Setting
	st7789_write_cmd(self, 0xB2);
	st7789_write_data(self, 0x0C);
	st7789_write_data(self, 0x0C);
	st7789_write_data(self, 0x00);
	st7789_write_data(self, 0x33);
	st7789_write_data(self, 0x33);

	/// Gate Control
	st7789_write_cmd(self, 0xB7);
	st7789_write_data(self, 0x00);

	/// VCOM Setting
	st7789_write_cmd(self, 0xBB);
	st7789_write_data(self, 0x3F);

	/// LCM Control
	st7789_write_cmd(self, 0xC0);
	st7789_write_data(self, 0x2C);

	/// VDV and VRH Command Enable
	st7789_write_cmd(self, 0xC2);
	st7789_write_data(self, 0x01);

	/// VRH Set
	st7789_write_cmd(self, 0xC3);
	st7789_write_data(self, 0x0D);

	/// VDV Set
	st7789_write_cmd(self, 0xC4);
	st7789_write_data(self, 0x20);

	/// Frame Rate Control in Normal Mode
	st7789_write_cmd(self, 0xC6);
	st7789_write_data(self, 0x03);

	/// Power Control 1
	st7789_write_cmd(self, 0xD0);
	st7789_write_data(self, 0xA4);
	st7789_write_data(self, 0xA1);

	/// Positive Voltage Gamma Control
	st7789_write_cmd(self, 0xE0);
	st7789_write_data(self, 0xF0);
	st7789_write_data(self, 0x03);
	st7789_write_data(self, 0x09);
	st7789_write_data(self, 0x03);
	st7789_write_data(self, 0x03);
	st7789_write_data(self, 0x10);
	st7789_write_data(self, 0x2D);
	st7789_write_data(self, 0x43);
	st7789_write_data(self, 0x3F);
	st7789_write_data(self, 0x33);
	st7789_write_data(self, 0x0D);
	st7789_write_data(self, 0x0E);
	st7789_write_data(self, 0x29);
	st7789_write_data(self, 0x32);

	/// Negative Voltage Gamma Control
	st7789_write_cmd(self, 0xE1);
	st7789_write_data(self, 0xF0);
	st7789_write_data(self, 0x0C);
	st7789_write_data(self, 0x10);
	st7789_write_data(self, 0x0E);
	st7789_write_data(self, 0x0E);
	st7789_write_data(self, 0x0A);
	st7789_write_data(self, 0x2D);
	st7789_write_data(self, 0x33);
	st7789_write_data(self, 0x45);
	st7789_write_data(self, 0x3A);
	st7789_write_data(self, 0x14);
	st7789_write_data(self, 0x19);
	st7789_write_data(self, 0x31);
	st7789_write_data(self, 0x37);

	/// Display Inversion On
	st7789_write_cmd(self, 0x21);

	/// Sleep Out
	st7789_write_cmd(self, 0x11);

	/// wait for power stability
	delay(100);

	/// Display on
	st7789_write_cmd(self, 0x29);
}

void st7789_initialize(struct ST7789_Screen *self, struct ST7789_ScreenAdaptorInterface **adaptor) {
	memset(self, 0, sizeof(struct ST7789_Screen));

	self->drawing_board = &drawing_board_vtable;
	self->adaptor = adaptor;

	self->size.x = 240;
	self->size.y = 240;

	st7789_prepare(self);
}
