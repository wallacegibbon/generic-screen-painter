#include "sc_ssd1306.h"
#include <string.h>

#define SSD1306_CTRL_WRITE_CMD_SINGLE 0x80
#define SSD1306_CTRL_WRITE_CMD_MULTI 0x00
#define SSD1306_CTRL_WRITE_DATA_SINGLE 0xC0
#define SSD1306_CTRL_WRITE_DATA_MULTI 0x40

void ssd1306_draw_point(struct SSD1306_Screen *self, struct Point p, int color);
void ssd1306_size(struct SSD1306_Screen *self, struct Point *p);
void ssd1306_clear(struct SSD1306_Screen *self, int color);
void ssd1306_flush(struct SSD1306_Screen *self);

static const struct DrawingBoardInterface drawing_board_vtable = {
	.draw_point = (DrawingBoardDrawPoint) ssd1306_draw_point,
	.size = (DrawingBoardSize) ssd1306_size,
	.clear = (DrawingBoardClear) ssd1306_clear,
	.flush = (DrawingBoardFlush) ssd1306_flush
};

static inline void ssd1306_start_transmit(struct SSD1306_Screen *self) {
	(*self->adaptor)->start_transmit(self->adaptor);
}

static inline void ssd1306_stop_transmit(struct SSD1306_Screen *self) {
	(*self->adaptor)->stop_transmit(self->adaptor);
}

static inline void ssd1306_write_byte(struct SSD1306_Screen *self, uint8_t data) {
	(*self->adaptor)->write_byte(self->adaptor, data);
}

void ssd1306_data_single_byte(struct SSD1306_Screen *self, uint8_t data) {
	ssd1306_write_byte(self, SSD1306_CTRL_WRITE_DATA_SINGLE);
	ssd1306_write_byte(self, data);
}

void ssd1306_data_multi_byte_start(struct SSD1306_Screen *self) {
	ssd1306_write_byte(self, SSD1306_CTRL_WRITE_DATA_MULTI);
}

void ssd1306_cmd_single_byte(struct SSD1306_Screen *self, uint8_t data) {
	ssd1306_write_byte(self, SSD1306_CTRL_WRITE_CMD_SINGLE);
	ssd1306_write_byte(self, data);
}

void ssd1306_cmd_multi_byte_start(struct SSD1306_Screen *self) {
	ssd1306_write_byte(self, SSD1306_CTRL_WRITE_CMD_MULTI);
}

void ssd1306_fix_32row(struct SSD1306_Screen *self) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);

	ssd1306_write_byte(self, 0xA8);
	ssd1306_write_byte(self, 0x1F);
	ssd1306_write_byte(self, 0xDA);
	ssd1306_write_byte(self, 0x02);

	ssd1306_stop_transmit(self);
}

void ssd1306_set_direction_1(struct SSD1306_Screen *self) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);
	ssd1306_write_byte(self, 0xA0);
	ssd1306_write_byte(self, 0xC0);
	ssd1306_stop_transmit(self);
}

void ssd1306_set_direction_2(struct SSD1306_Screen *self) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);
	ssd1306_write_byte(self, 0xA1);
	ssd1306_write_byte(self, 0xC8);
	ssd1306_stop_transmit(self);
}

void ssd1306_set_brightness(struct SSD1306_Screen *self, uint8_t value) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);
	ssd1306_write_byte(self, 0x81);
	ssd1306_write_byte(self, value);
	ssd1306_stop_transmit(self);
}

void ssd1306_color_reverse(struct SSD1306_Screen *self) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_single_byte(self, 0xA7);
	ssd1306_stop_transmit(self);
}

void ssd1306_display_on(struct SSD1306_Screen *self) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);
	/// turn on the charge pump
	ssd1306_write_byte(self, 0x8D);
	ssd1306_write_byte(self, 0x14);
	/// turn on display
	ssd1306_write_byte(self, 0xAF);
	ssd1306_stop_transmit(self);
}

void ssd1306_display_off(struct SSD1306_Screen *self) {
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);
	/// turn on the charge pump
	ssd1306_write_byte(self, 0x8D);
	ssd1306_write_byte(self, 0x10);
	/// turn on display
	ssd1306_write_byte(self, 0xAE);
	ssd1306_stop_transmit(self);
}

void ssd1306_draw_cell(struct SSD1306_Screen *self, int x, int page_idx, int cell_value) {
	ssd1306_start_transmit(self);

	ssd1306_cmd_single_byte(self, 0xB0 + page_idx);
	ssd1306_cmd_single_byte(self, ((x >> 4) & 0x0F) | 0x10);
	ssd1306_cmd_single_byte(self, x & 0x0F);
	ssd1306_data_single_byte(self, cell_value);

	ssd1306_stop_transmit(self);
}

void ssd1306_draw_point(struct SSD1306_Screen *self, struct Point p, int color) {
	int page_idx, byte_idx, tmp;

	if (p.x >= self->size.x || p.y >= self->size.y)
		return;

	/// SSD1306 is mono color, this can handle wrong arguements
	color = color ? 1 : 0;

	page_idx = p.y / 8;
	byte_idx = p.y % 8;

	tmp = self->buffer[p.x][page_idx];
	tmp &= ~(1 << byte_idx);
	tmp |= color << byte_idx;

	self->buffer[p.x][page_idx] = tmp;

	if (self->auto_flush)
		ssd1306_draw_cell(self, p.x, page_idx, tmp);
}

int ssd1306_page_byte(struct SSD1306_Screen *self, int page_index, int x) {
	return self->buffer[x][page_index];
}

int ssd1306_page_byte_empty(struct SSD1306_Screen *self, int page_index, int x) {
	return self->clear_color;
}

/// This function will send data to SSD1306 through IIC directly as it is
/// designed to speed up operations. The `auto_flush` is ignored here.
void ssd1306_iterate_page(struct SSD1306_Screen *self, int page_index, int (*fn)(struct SSD1306_Screen *, int, int)) {
	int x;

	ssd1306_start_transmit(self);
	ssd1306_cmd_single_byte(self, 0xB0 + page_index);
	ssd1306_cmd_single_byte(self, 0x00);
	ssd1306_cmd_single_byte(self, 0x10);

	ssd1306_data_multi_byte_start(self);

	for (x = 0; x < 128; x++)
		ssd1306_write_byte(self, fn(self, page_index, x));

	ssd1306_stop_transmit(self);
}

void ssd1306_iterate(struct SSD1306_Screen *self, int (*fn)(struct SSD1306_Screen *, int, int)) {
	int page;
	for (page = 0; page < 8; page++)
		ssd1306_iterate_page(self, page, fn);
}

void ssd1306_size(struct SSD1306_Screen *self, struct Point *p) {
	point_initialize(p, self->size.x, self->size.y);
}

void ssd1306_flush(struct SSD1306_Screen *self) {
	ssd1306_iterate(self, ssd1306_page_byte);
}

void ssd1306_clear(struct SSD1306_Screen *self, int color) {
	/// `self->clear_color` represents a page, not a point.
	self->clear_color = color ? 0xFF : 0;
	ssd1306_iterate(self, ssd1306_page_byte_empty);
}

void ssd1306_prepare(struct SSD1306_Screen *self) {
	if (self->direction)
		ssd1306_set_direction_2(self);
	else
		ssd1306_set_direction_1(self);

	/*
	ssd1306_start_transmit(self);
	ssd1306_cmd_multi_byte_start(self);

	/// vertical shift, 0 ~ 63
	ssd1306_write_byte(self, 0xD3);
	ssd1306_write_byte(self, 0x20);

	/// Ratio/Oscillator & Clock Divide
	ssd1306_write_byte(self, 0xD5);
	ssd1306_write_byte(self, 0xF0);

	ssd1306_write_byte(self, 0xD9);
	ssd1306_write_byte(self, 0x22);

	ssd1306_stop_transmit(self);
	*/
}

void ssd1306_set_up_down_invert(struct SSD1306_Screen *self) {
	if (self->direction)
		ssd1306_set_direction_1(self);
	else
		ssd1306_set_direction_2(self);

	self->direction = !self->direction;
}

void ssd1306_initialize(struct SSD1306_Screen *self, struct SSD1306_ScreenAdaptorInterface **adaptor) {
	memset(self, 0, sizeof(struct SSD1306_Screen));

	self->drawing_board = &drawing_board_vtable;
	self->adaptor = adaptor;

	self->size.x = 128;
	self->size.y = 64;
	self->auto_flush = 0;
	self->half_mode = 0;

	ssd1306_prepare(self);
}

