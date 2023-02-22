#include "ssd1306.h"
#include <string.h>

#define SSD1306_CTRL_WRITE_CMD_SINGLE 0x80
#define SSD1306_CTRL_WRITE_CMD_MULTI 0x00
#define SSD1306_CTRL_WRITE_DATA_SINGLE 0xC0
#define SSD1306_CTRL_WRITE_DATA_MULTI 0x40

void SSD1306_Screen_start_transmit(struct SSD1306_Screen *self) {
	self->adaptor->start_transmit(self->adaptor);
}

void SSD1306_Screen_stop_transmit(struct SSD1306_Screen *self) {
	self->adaptor->stop_transmit(self->adaptor);
}

void SSD1306_Screen_write_byte(struct SSD1306_Screen *self, uint8_t data) {
	self->adaptor->write_byte(self->adaptor, data);
}

void SSD1306_Screen_data_single_byte(
	struct SSD1306_Screen *self, uint8_t data
) {
	SSD1306_Screen_write_byte(self, SSD1306_CTRL_WRITE_DATA_SINGLE);
	SSD1306_Screen_write_byte(self, data);
}

void SSD1306_Screen_data_multi_byte_start(struct SSD1306_Screen *self) {
	SSD1306_Screen_write_byte(self, SSD1306_CTRL_WRITE_DATA_MULTI);
}

void SSD1306_Screen_cmd_single_byte(
	struct SSD1306_Screen *self, uint8_t data
) {
	SSD1306_Screen_write_byte(self, SSD1306_CTRL_WRITE_CMD_SINGLE);
	SSD1306_Screen_write_byte(self, data);
}

void SSD1306_Screen_cmd_multi_byte_start(struct SSD1306_Screen *self) {
	SSD1306_Screen_write_byte(self, SSD1306_CTRL_WRITE_CMD_MULTI);
}

void SSD1306_Screen_fix_32row(struct SSD1306_Screen *self) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_multi_byte_start(self);

	SSD1306_Screen_write_byte(self, 0xA8);
	SSD1306_Screen_write_byte(self, 0x1F);
	SSD1306_Screen_write_byte(self, 0xDA);
	SSD1306_Screen_write_byte(self, 0x02);

	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_set_up_down_invert(struct SSD1306_Screen *self) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_multi_byte_start(self);
	if (self->direction) {
		SSD1306_Screen_write_byte(self, 0xA0);
		SSD1306_Screen_write_byte(self, 0xC0);
	} else {
		SSD1306_Screen_write_byte(self, 0xA1);
		SSD1306_Screen_write_byte(self, 0xC8);
	}
	self->direction = !self->direction;
	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_set_brightness(
	struct SSD1306_Screen *self, uint8_t value
) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_multi_byte_start(self);
	SSD1306_Screen_write_byte(self, 0x81);
	SSD1306_Screen_write_byte(self, value);
	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_color_reverse(struct SSD1306_Screen *self) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_single_byte(self, 0xA7);
	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_display_on(struct SSD1306_Screen *self) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_multi_byte_start(self);
	/// turn on the charge pump
	SSD1306_Screen_write_byte(self, 0x8D);
	SSD1306_Screen_write_byte(self, 0x14);
	/// turn on display
	SSD1306_Screen_write_byte(self, 0xAF);
	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_display_off(struct SSD1306_Screen *self) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_multi_byte_start(self);
	/// turn on the charge pump
	SSD1306_Screen_write_byte(self, 0x8D);
	SSD1306_Screen_write_byte(self, 0x10);
	/// turn on display
	SSD1306_Screen_write_byte(self, 0xAE);
	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_draw_cell(
	struct SSD1306_Screen *self, int x, int page_idx, int cell_value
) {
	SSD1306_Screen_start_transmit(self);

	SSD1306_Screen_cmd_single_byte(self, 0xB0 + page_idx);
	SSD1306_Screen_cmd_single_byte(self, ((x >> 4) & 0x0F) | 0x10);
	SSD1306_Screen_cmd_single_byte(self, x & 0x0F);
	SSD1306_Screen_data_single_byte(self, cell_value);

	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_draw_point(
	struct SSD1306_Screen *self, struct Point p, int color
) {
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
		SSD1306_Screen_draw_cell(self, p.x, page_idx, tmp);
}

int SSD1306_Screen_page_byte(
	struct SSD1306_Screen *self, int page_index, int x
) {
	return self->buffer[x][page_index];
}

int SSD1306_Screen_page_byte_empty(
	struct SSD1306_Screen *self, int page_index, int x
) {
	return self->clear_color;
}

void SSD1306_Screen_iterate_page(
	struct SSD1306_Screen *self,
	int page_index,
	int (*fn)(struct SSD1306_Screen *, int, int)
) {
	int x;

	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_single_byte(self, 0xB0 + page_index);
	SSD1306_Screen_cmd_single_byte(self, 0x00);
	SSD1306_Screen_cmd_single_byte(self, 0x10);

	SSD1306_Screen_data_multi_byte_start(self);

	for (x = 0; x < 128; x++)
		SSD1306_Screen_write_byte(self, fn(self, page_index, x));

	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_iterate(
	struct SSD1306_Screen *self,
	int (*fn)(struct SSD1306_Screen *, int, int)
) {
	int page;
	for (page = 0; page < 8; page++)
		SSD1306_Screen_iterate_page(self, page, fn);
}

void SSD1306_Screen_size(struct SSD1306_Screen *self, struct Point *p) {
	Point_initialize(p, self->size.x, self->size.y);
}

void SSD1306_Screen_flush(struct SSD1306_Screen *self) {
	SSD1306_Screen_iterate(self, SSD1306_Screen_page_byte);
}

void SSD1306_Screen_clear(struct SSD1306_Screen *self, int color) {
	/// clear_color represents a page, not a point
	self->clear_color = color ? 0xFF : 0;
	SSD1306_Screen_iterate(self, SSD1306_Screen_page_byte_empty);
}

void SSD1306_Screen_prepare(struct SSD1306_Screen *self) {
	SSD1306_Screen_start_transmit(self);
	SSD1306_Screen_cmd_multi_byte_start(self);

	/// normal direction, can be changed by method `up_down_invert`
	SSD1306_Screen_write_byte(self, 0xA0);
	SSD1306_Screen_write_byte(self, 0xC0);

	/// vertical shift, 0 ~ 63
	//SSD1306_Screen_write_byte(self, 0xD3);
	//SSD1306_Screen_write_byte(self, 0x20);

	/// Ratio/Oscillator & Clock Divide
	//SSD1306_Screen_write_byte(self, 0xD5);
	//SSD1306_Screen_write_byte(self, 0xF0);

	//SSD1306_Screen_write_byte(self, 0xD9);
	//SSD1306_Screen_write_byte(self, 0x22);

	SSD1306_Screen_stop_transmit(self);
}

void SSD1306_Screen_initialize(
	struct SSD1306_Screen *self,
	struct SSD1306_ScreenAdaptorInterface *adaptor
) {
	memset(self, 0, sizeof(struct SSD1306_Screen));
	PainterInterface_initialize(&self->painter_interface);

	self->painter_interface.draw_point =
		(PainterDrawPoint) SSD1306_Screen_draw_point;

	self->painter_interface.size = (PainterSize) SSD1306_Screen_size;
	self->painter_interface.clear = (PainterClear) SSD1306_Screen_clear;
	self->painter_interface.flush = (PainterFlush) SSD1306_Screen_flush;

	self->adaptor = adaptor;

	self->size.x = 128;
	self->size.y = 64;

	SSD1306_Screen_prepare(self);
}

