#ifndef __SC_ST7789_H
#define __SC_ST7789_H

#include "sc_common.h"
#include "sc_painter.h"
#include <stdint.h>

typedef void (*st7789_adaptor_write_data_16_fn)(void *adaptor, uint16_t data);
typedef void (*st7789_adaptor_write_data_fn)(void *adaptor, uint8_t data);
typedef void (*st7789_adaptor_write_cmd_fn)(void *adaptor, uint8_t data);

struct st7789_adaptor_i {
	st7789_adaptor_write_data_16_fn write_data_16;
	st7789_adaptor_write_data_fn write_data;
	st7789_adaptor_write_cmd_fn write_cmd;
};

struct st7789_screen {
	struct drawing_i *drawing_board;
	struct st7789_adaptor_i **adaptor;
	struct point size;
};

void st7789_initialize(struct st7789_screen *self, struct st7789_adaptor_i **adaptor);
void st7789_set_address(struct st7789_screen *self, struct point p1, struct point p2);

#endif
