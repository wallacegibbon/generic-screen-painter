#ifndef __SC_ST7789_H
#define __SC_ST7789_H

#include "sc_painter.h"
#include "sc_common.h"
#include <stdint.h>

typedef void (*ST7789_ScreenAdaptorWriteData16)(void *adaptor, uint16_t data);
typedef void (*ST7789_ScreenAdaptorWriteData)(void *adaptor, uint8_t data);
typedef void (*ST7789_ScreenAdaptorWriteCmd)(void *adaptor, uint8_t data);

struct ST7789_ScreenAdaptorInterface {
	ST7789_ScreenAdaptorWriteData16 write_data_16;
	ST7789_ScreenAdaptorWriteData write_data;
	ST7789_ScreenAdaptorWriteCmd write_cmd;
};

struct ST7789_Screen {
	const struct DrawingBoardInterface *drawing_board;
	struct ST7789_ScreenAdaptorInterface **adaptor;
	struct Point size;
};

void ST7789_Screen_initialize(
	struct ST7789_Screen *self,
	struct ST7789_ScreenAdaptorInterface **adaptor
);

void ST7789_Screen_set_address(
	struct ST7789_Screen *self, struct Point p1, struct Point p2
);

#endif

