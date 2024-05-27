#include "sc_ssd1306_i2c.h"
#include "sc_adaptor.h"
#include "sc_color.h"
#include "sc_painter.h"
#include <stdarg.h>
#include <string.h>

#define SSD1306_CTRL_WRITE_CMD_SINGLE 0x80
#define SSD1306_CTRL_WRITE_CMD_MULTI 0x00
#define SSD1306_CTRL_WRITE_DATA_SINGLE 0xC0
#define SSD1306_CTRL_WRITE_DATA_MULTI 0x40

int ssd1306_draw_point(struct ssd1306_i2c_screen *self, struct point p, unsigned long color);
int ssd1306_size(struct ssd1306_i2c_screen *self, struct point *p);
int ssd1306_clear(struct ssd1306_i2c_screen *self, unsigned long color);
int ssd1306_flush(struct ssd1306_i2c_screen *self);

static struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn_t)ssd1306_draw_point,
	.size = (drawing_size_fn_t)ssd1306_size,
	.clear = (drawing_clear_fn_t)ssd1306_clear,
	.flush = (drawing_flush_fn_t)ssd1306_flush,
};

int ssd1306_data_single_byte(struct ssd1306_i2c_screen *self, int data)
{
	if (sc_adaptor_i2c_write_byte(self->adaptor, SSD1306_CTRL_WRITE_DATA_SINGLE))
		return 1;
	if (sc_adaptor_i2c_write_byte(self->adaptor, data))
		return 2;
	return 0;
}

int ssd1306_data_multi_byte_start(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_write_byte(self->adaptor, SSD1306_CTRL_WRITE_DATA_MULTI))
		return 1;
	return 0;
}

int ssd1306_cmd_single_byte(struct ssd1306_i2c_screen *self, int data)
{
	if (sc_adaptor_i2c_write_byte(self->adaptor, SSD1306_CTRL_WRITE_CMD_SINGLE))
		return 1;
	if (sc_adaptor_i2c_write_byte(self->adaptor, data))
		return 2;
	return 0;
}

int ssd1306_cmd_multi_byte_start(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_write_byte(self->adaptor, SSD1306_CTRL_WRITE_CMD_MULTI))
		return 1;
	return 0;
}

static int ssd1306_cmd(struct ssd1306_i2c_screen *self, int arg_cnt, ...)
{
	va_list args;
	int i;

	va_start(args, arg_cnt);

	if (ssd1306_cmd_multi_byte_start(self))
		return 1;
	for (i = 0; i < arg_cnt; i++) {
		if (sc_adaptor_i2c_write_byte(self->adaptor, va_arg(args, int)))
			return 2;
	}

	va_end(args);
	return 0;
}

int ssd1306_fix_32row(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd(self, 4, 0xA8, 0x1F, 0xDA, 0x02))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_set_direction_1(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd(self, 2, 0xA0, 0xC0))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_set_direction_2(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd(self, 2, 0xA1, 0xC8))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_set_brightness(struct ssd1306_i2c_screen *self, int value)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd(self, 2, 0x81, value))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_color_reverse(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd_single_byte(self, 0xA7))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_display_on(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd(self, 3, /* turn on the charge pump */ 0x8D, 0x14, /* turn on display */ 0xAF))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_display_off(struct ssd1306_i2c_screen *self)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd(self, 3, /* turn off the charge pump */ 0x8D, 0x10, /* turn off display */ 0xAE))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_draw_cell(struct ssd1306_i2c_screen *self, int x, int page_idx, int cell_value)
{
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd_single_byte(self, 0xB0 + page_idx))
		return 1;

	/// Set Higher Column Start Address for Page Addressing Mode
	if (ssd1306_cmd_single_byte(self, ((x >> 4) & 0x0F) | 0x10))
		return 2;

	/// Set Lower Column Start Address for Page Addressing Mode
	if (ssd1306_cmd_single_byte(self, x & 0x0F))
		return 3;

	if (ssd1306_data_single_byte(self, cell_value))
		return 4;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_draw_point(struct ssd1306_i2c_screen *self, struct point p, unsigned long color)
{
	int page_idx, byte_idx, tmp;

	if (p.x >= self->size.x || p.y >= self->size.y)
		return 1;

	color = color_to_1bit(color);

	page_idx = p.y / 8;
	byte_idx = p.y % 8;

	tmp = self->buffer[p.x][page_idx];
	tmp &= ~(1 << byte_idx);
	tmp |= color << byte_idx;

	self->buffer[p.x][page_idx] = tmp;

	if (self->auto_flush) {
		if (ssd1306_draw_cell(self, p.x, page_idx, tmp))
			return 2;
	}

	return 0;
}

int ssd1306_page_byte(struct ssd1306_i2c_screen *self, int page_index, int x)
{
	return self->buffer[x][page_index];
}

int ssd1306_page_byte_empty(struct ssd1306_i2c_screen *self, int page_index, int x)
{
	return self->clear_color;
}

/// This function will send data to SSD1306 through IIC directly as it is
/// designed to speed up operations. The `auto_flush` is ignored here.
int ssd1306_iterate_page(struct ssd1306_i2c_screen *self, int page_index,
			 int (*fn)(struct ssd1306_i2c_screen *, int, int))
{
	int x;

	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	if (ssd1306_cmd_single_byte(self, 0xB0 + page_index))
		return 1;
	if (ssd1306_cmd_single_byte(self, 0x00))
		return 2;
	if (ssd1306_cmd_single_byte(self, 0x10))
		return 3;

	if (ssd1306_data_multi_byte_start(self))
		return 4;

	for (x = 0; x < 128; x++) {
		if (sc_adaptor_i2c_write_byte(self->adaptor, fn(self, page_index, x)))
			return 9;
	}

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;

	return 0;
}

int ssd1306_iterate(struct ssd1306_i2c_screen *self, int (*fn)(struct ssd1306_i2c_screen *, int, int))
{
	int page;
	for (page = 0; page < 8; page++) {
		if (ssd1306_iterate_page(self, page, fn))
			return page + 10;
	}
	return 0;
}

int ssd1306_size(struct ssd1306_i2c_screen *self, struct point *p)
{
	point_init(p, self->size.x, self->size.y);
	return 0;
}

int ssd1306_flush(struct ssd1306_i2c_screen *self)
{
	return ssd1306_iterate(self, ssd1306_page_byte);
}

int ssd1306_clear(struct ssd1306_i2c_screen *self, unsigned long color)
{
	/// `self->clear_color` represents a page, not a point.
	self->clear_color = color ? 0xFF : 0;
	return ssd1306_iterate(self, ssd1306_page_byte_empty);
}

int ssd1306_prepare(struct ssd1306_i2c_screen *self)
{
	if (self->direction)
		ssd1306_set_direction_2(self);
	else
		ssd1306_set_direction_1(self);

	/*
	if (sc_adaptor_i2c_start_transmit(self->adaptor))
		return 0xFE;

	/// vertical shift 0 ~ 63, Ratio/Oscillator & Clock Divide
	if (ssd1306_cmd(self, 6, 0xD3, 0x20, 0xD5, 0xF0, 0xD9, 0x22))
		return 1;

	if (sc_adaptor_i2c_stop_transmit(self->adaptor))
		return 0xFF;
	*/

	return 0;
}

int ssd1306_set_up_down_invert(struct ssd1306_i2c_screen *self)
{
	if (self->direction)
		ssd1306_set_direction_1(self);
	else
		ssd1306_set_direction_2(self);

	self->direction = !self->direction;
	return 0;
}

int ssd1306_init(struct ssd1306_i2c_screen *self, struct sc_byte_adaptor_i **adaptor)
{
	memset(self, 0, sizeof(struct ssd1306_i2c_screen));

	self->drawing_board = &drawing_interface;
	self->adaptor = adaptor;

	self->size.x = 128;
	self->size.y = 64;
	self->auto_flush = 0;
	self->half_mode = 0;

	ssd1306_prepare(self);
	return 0;
}
