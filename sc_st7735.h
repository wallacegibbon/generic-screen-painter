#ifndef __SC_ST7735_H
#define __SC_ST7735_H

#include "sc_common.h"
#include "sc_painter.h"
#include <stdint.h>

typedef void (*st7735_adaptor_write_data_16_fn)(void *adaptor, uint16_t data);
typedef void (*st7735_adaptor_write_data_fn)(void *adaptor, uint8_t data);
typedef void (*st7735_adaptor_write_cmd_fn)(void *adaptor, uint8_t data);

struct st7735_adaptor_i {
	st7735_adaptor_write_data_16_fn write_data_16;
	st7735_adaptor_write_data_fn write_data;
	st7735_adaptor_write_cmd_fn write_cmd;
};

struct st7735_screen {
	const struct drawing_i *drawing_board;
	struct st7735_adaptor_i **adaptor;
	struct point size;
};

void st7735_initialize(struct st7735_screen *self, struct st7735_adaptor_i **adaptor);
void st7735_set_address(struct st7735_screen *self, struct point p1, struct point p2);

#endif
