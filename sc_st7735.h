#ifndef __ST7735_H
#define __ST7735_H

#include "sc_common.h"
#include "sc_painter.h"
#include <stdint.h>

typedef void (*ST7735_ScreenAdaptorWriteData16)(void *adaptor, uint16_t data);
typedef void (*ST7735_ScreenAdaptorWriteData)(void *adaptor, uint8_t data);
typedef void (*ST7735_ScreenAdaptorWriteCmd)(void *adaptor, uint8_t data);

struct ST7735_ScreenAdaptorInterface {
	ST7735_ScreenAdaptorWriteData16 write_data_16;
	ST7735_ScreenAdaptorWriteData write_data;
	ST7735_ScreenAdaptorWriteCmd write_cmd;
};

struct ST7735_Screen {
	const struct DrawingBoardInterface *drawing_board;
	struct ST7735_ScreenAdaptorInterface **adaptor;
	struct Point size;
};

void st7735_initialize(struct ST7735_Screen *self, struct ST7735_ScreenAdaptorInterface **adaptor);
void st7735_set_address(struct ST7735_Screen *self, struct Point p1, struct Point p2);

#endif
