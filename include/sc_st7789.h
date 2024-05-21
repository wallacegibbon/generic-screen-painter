#ifndef __SC_ST7789_H
#define __SC_ST7789_H

#include "sc_common.h"
#include "sc_painter.h"

typedef int (*st7789_adaptor_write_data_16_fn_t)(void *adaptor, int data);

typedef int (*st7789_adaptor_write_data_fn_t)(void *adaptor, int data);

typedef int (*st7789_adaptor_write_cmd_fn_t)(void *adaptor, int data);

struct st7789_adaptor_i {
	st7789_adaptor_write_data_16_fn_t write_data_16;
	st7789_adaptor_write_data_fn_t write_data;
	st7789_adaptor_write_cmd_fn_t write_cmd;
};

struct st7789_screen {
	struct drawing_i *drawing_board;
	struct st7789_adaptor_i **adaptor;
	struct point size;
};

int st7789_init(struct st7789_screen *self, struct st7789_adaptor_i **adaptor);

int st7789_set_address(struct st7789_screen *self, struct point p1, struct point p2);

#endif
