#include "sc_st7789.h"
#include "sc_point_iterator.h"
#include <stddef.h>
#include <string.h>

void ST7789_Screen_draw_point(struct ST7789_Screen *self, struct Point p, int color);
void ST7789_Screen_size(struct ST7789_Screen *self, struct Point *p);
void ST7789_Screen_fill(struct ST7789_Screen *self, struct Point p1, struct Point p2, int color);

static const struct DrawingBoardInterface drawing_board_vtable = {
	.draw_point = (DrawingBoardDrawPoint) ST7789_Screen_draw_point,
	.size = (DrawingBoardSize) ST7789_Screen_size,
	.fill = (DrawingBoardFill) ST7789_Screen_fill
};

static inline void ST7789_Screen_write_data_16(struct ST7789_Screen *self, uint16_t data) {
	(*self->adaptor)->write_data_16(self->adaptor, data);
}

static inline void ST7789_Screen_write_data(struct ST7789_Screen *self, uint8_t data) {
	(*self->adaptor)->write_data(self->adaptor, data);
}

static inline void ST7789_Screen_write_cmd(struct ST7789_Screen *self, uint8_t data) {
	(*self->adaptor)->write_cmd(self->adaptor, data);
}

void ST7789_Screen_set_address(struct ST7789_Screen *self, struct Point p1, struct Point p2) {
	/// column address settings
	ST7789_Screen_write_cmd(self, 0x2A);
	ST7789_Screen_write_data_16(self, p1.x);
	ST7789_Screen_write_data_16(self, p2.x);

	/// row address setting
	ST7789_Screen_write_cmd(self, 0x2B);
	ST7789_Screen_write_data_16(self, p1.y);
	ST7789_Screen_write_data_16(self, p2.y);

	/// memory write
	ST7789_Screen_write_cmd(self, 0x2C);
}

void ST7789_Screen_draw_point(struct ST7789_Screen *self, struct Point p, int color) {
	if (p.x >= self->size.x || p.y >= self->size.y) return;

	ST7789_Screen_set_address(self, p, p);
	ST7789_Screen_write_data_16(self, (uint16_t) color);
}

void ST7789_Screen_size(struct ST7789_Screen *self, struct Point *p) {
	Point_initialize(p, self->size.x, self->size.y);
}

/// The default `fill` calls `draw_point`, which will cause many
/// unnecessary `set_address` invocations.
void ST7789_Screen_fill(struct ST7789_Screen *self, struct Point p1, struct Point p2, int color) {
	int n = ABS((p1.x - p2.x) * (p1.y - p2.y));

	ST7789_Screen_set_address(self, p1, p2);
	while (n--)
		ST7789_Screen_write_data_16(self, (uint16_t) color);
}

void ST7789_Screen_prepare(struct ST7789_Screen *self) {
	/// Memory Data Access Control
	ST7789_Screen_write_cmd(self, 0x36);
	ST7789_Screen_write_data(self, 0x00);

	/// RGB 5-6-5-bit
	ST7789_Screen_write_cmd(self, 0x3A);
	ST7789_Screen_write_data(self, 0x05);

	/// Porch Setting
	ST7789_Screen_write_cmd(self, 0xB2);
	ST7789_Screen_write_data(self, 0x0C);
	ST7789_Screen_write_data(self, 0x0C);
	ST7789_Screen_write_data(self, 0x00);
	ST7789_Screen_write_data(self, 0x33);
	ST7789_Screen_write_data(self, 0x33);

	/// Gate Control
	ST7789_Screen_write_cmd(self, 0xB7);
	ST7789_Screen_write_data(self, 0x00);

	/// VCOM Setting
	ST7789_Screen_write_cmd(self, 0xBB);
	ST7789_Screen_write_data(self, 0x3F);

	/// LCM Control
	ST7789_Screen_write_cmd(self, 0xC0);
	ST7789_Screen_write_data(self, 0x2C);

	/// VDV and VRH Command Enable
	ST7789_Screen_write_cmd(self, 0xC2);
	ST7789_Screen_write_data(self, 0x01);

	/// VRH Set
	ST7789_Screen_write_cmd(self, 0xC3);
	ST7789_Screen_write_data(self, 0x0D);

	/// VDV Set
	ST7789_Screen_write_cmd(self, 0xC4);
	ST7789_Screen_write_data(self, 0x20);

	/// Frame Rate Control in Normal Mode
	ST7789_Screen_write_cmd(self, 0xC6);
	ST7789_Screen_write_data(self, 0x03);

	/// Power Control 1
	ST7789_Screen_write_cmd(self, 0xD0);
	ST7789_Screen_write_data(self, 0xA4);
	ST7789_Screen_write_data(self, 0xA1);

	/// Positive Voltage Gamma Control
	ST7789_Screen_write_cmd(self, 0xE0);
	ST7789_Screen_write_data(self, 0xF0);
	ST7789_Screen_write_data(self, 0x03);
	ST7789_Screen_write_data(self, 0x09);
	ST7789_Screen_write_data(self, 0x03);
	ST7789_Screen_write_data(self, 0x03);
	ST7789_Screen_write_data(self, 0x10);
	ST7789_Screen_write_data(self, 0x2D);
	ST7789_Screen_write_data(self, 0x43);
	ST7789_Screen_write_data(self, 0x3F);
	ST7789_Screen_write_data(self, 0x33);
	ST7789_Screen_write_data(self, 0x0D);
	ST7789_Screen_write_data(self, 0x0E);
	ST7789_Screen_write_data(self, 0x29);
	ST7789_Screen_write_data(self, 0x32);

	/// Negative Voltage Gamma Control
	ST7789_Screen_write_cmd(self, 0xE1);
	ST7789_Screen_write_data(self, 0xF0);
	ST7789_Screen_write_data(self, 0x0C);
	ST7789_Screen_write_data(self, 0x10);
	ST7789_Screen_write_data(self, 0x0E);
	ST7789_Screen_write_data(self, 0x0E);
	ST7789_Screen_write_data(self, 0x0A);
	ST7789_Screen_write_data(self, 0x2D);
	ST7789_Screen_write_data(self, 0x33);
	ST7789_Screen_write_data(self, 0x45);
	ST7789_Screen_write_data(self, 0x3A);
	ST7789_Screen_write_data(self, 0x14);
	ST7789_Screen_write_data(self, 0x19);
	ST7789_Screen_write_data(self, 0x31);
	ST7789_Screen_write_data(self, 0x37);

	/// Display Inversion On
	ST7789_Screen_write_cmd(self, 0x21);

	/// Sleep Out
	ST7789_Screen_write_cmd(self, 0x11);

	/// wait for power stability
	delay(100);

	/// Display on
	ST7789_Screen_write_cmd(self, 0x29);
}

void ST7789_Screen_initialize(struct ST7789_Screen *self, struct ST7789_ScreenAdaptorInterface **adaptor) {
	memset(self, 0, sizeof(struct ST7789_Screen));

	self->drawing_board = &drawing_board_vtable;
	self->adaptor = adaptor;

	self->size.x = 240;
	self->size.y = 240;

	ST7789_Screen_prepare(self);
}

