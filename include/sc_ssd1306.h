#ifndef __SC_SSD1306_H
#define __SC_SSD1306_H

#include "sc_painter.h"

typedef int (*ssd1306_adaptor_start_transmit_fn_t)(void *adaptor);
typedef int (*ssd1306_adaptor_stop_transmit_fn_t)(void *adaptor);
typedef int (*ssd1306_adaptor_write_byte_fn_t)(void *adaptor, int data);

struct ssd1306_adaptor_i {
	ssd1306_adaptor_start_transmit_fn_t start_transmit;
	ssd1306_adaptor_stop_transmit_fn_t stop_transmit;
	ssd1306_adaptor_write_byte_fn_t write_byte;
};

struct ssd1306_screen {
	struct drawing_i *drawing_board;
	struct ssd1306_adaptor_i **adaptor;
	struct point size;
	/// just to hold some value since we don't have closure in C.
	unsigned long clear_color;
	unsigned char direction;
	unsigned char auto_flush;
	/// SSD1306 support 128x32 mode
	unsigned char half_mode;
	unsigned char buffer[128][8];
};

int ssd1306_init(struct ssd1306_screen *self, struct ssd1306_adaptor_i **adaptor);

int ssd1306_fix_32row(struct ssd1306_screen *self);

int ssd1306_set_brightness(struct ssd1306_screen *self, int value);

int ssd1306_set_up_down_invert(struct ssd1306_screen *self);

int ssd1306_color_reverse(struct ssd1306_screen *self);

int ssd1306_display_on(struct ssd1306_screen *self);

int ssd1306_display_off(struct ssd1306_screen *self);

#endif
