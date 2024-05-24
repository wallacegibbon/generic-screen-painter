#ifndef __SC_SSD1306_I2C_H
#define __SC_SSD1306_I2C_H

#include "sc_adaptor.h"
#include "sc_painter.h"

struct ssd1306_i2c_screen {
	struct drawing_i *drawing_board;
	struct sc_adaptor_i2c_i **adaptor;
	struct point size;
	/// just to hold some value since we don't have closure in C.
	unsigned long clear_color;
	unsigned char direction;
	unsigned char auto_flush;
	/// SSD1306 support 128x32 mode
	unsigned char half_mode;
	unsigned char buffer[128][8];
};

int ssd1306_init(struct ssd1306_i2c_screen *self, struct sc_adaptor_i2c_i **adaptor);

int ssd1306_fix_32row(struct ssd1306_i2c_screen *self);

int ssd1306_set_brightness(struct ssd1306_i2c_screen *self, int value);

int ssd1306_set_up_down_invert(struct ssd1306_i2c_screen *self);

int ssd1306_color_reverse(struct ssd1306_i2c_screen *self);

int ssd1306_display_on(struct ssd1306_i2c_screen *self);

int ssd1306_display_off(struct ssd1306_i2c_screen *self);

#endif
