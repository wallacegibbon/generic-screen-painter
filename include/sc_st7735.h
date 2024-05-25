#ifndef __SC_ST7735_H
#define __SC_ST7735_H

#include "sc_adaptor.h"
#include "sc_common.h"
#include "sc_painter.h"

struct st7735_screen {
	struct drawing_i *drawing_board;
	struct sc_adaptor_i **adaptor;
	struct point size;
};

int st7735_init(struct st7735_screen *self, struct sc_adaptor_i **adaptor, struct point size);

int st7735_set_address(struct st7735_screen *self, struct point p1, struct point p2);

#endif
