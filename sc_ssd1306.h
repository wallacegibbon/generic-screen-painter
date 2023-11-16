#ifndef __SC_SSD1306_H
#define __SC_SSD1306_H

#include "sc_painter.h"
#include <stdint.h>

typedef void (*ssd1306_adaptor_start_transmit_fn)(void *adaptor);
typedef void (*ssd1306_adaptor_stop_transmit_fn)(void *adaptor);
typedef void (*ssd1306_adaptor_write_byte_fn)(void *adaptor, uint8_t data);

struct ssd1306_adaptor_i {
	ssd1306_adaptor_start_transmit_fn start_transmit;
	ssd1306_adaptor_stop_transmit_fn stop_transmit;
	ssd1306_adaptor_write_byte_fn write_byte;
};

struct ssd1306_screen {
	const struct drawing_i *drawing_board;
	struct ssd1306_adaptor_i **adaptor;
	struct point size;
	/// just to hold some value since we don't have closure in C.
	int clear_color;
	uint8_t direction;
	uint8_t auto_flush;
	/// SSD1306 support 128x32 mode
	uint8_t half_mode;
	uint8_t buffer[128][8];
};

void ssd1306_initialize(struct ssd1306_screen *self, struct ssd1306_adaptor_i **adaptor);
void ssd1306_fix_32row(struct ssd1306_screen *self);
void ssd1306_set_brightness(struct ssd1306_screen *self, uint8_t value);
void ssd1306_set_up_down_invert(struct ssd1306_screen *self);
void ssd1306_color_reverse(struct ssd1306_screen *self);
void ssd1306_display_on(struct ssd1306_screen *self);
void ssd1306_display_off(struct ssd1306_screen *self);

#endif
