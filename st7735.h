#ifndef __ST7735_H
#define __ST7735_H

#include "painter.h"
#include "common.h"
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
	struct PainterInterface painter_interface;
	struct ST7735_ScreenAdaptorInterface *adaptor;
	struct Point size;
};

void ST7735_Screen_initialize(
	struct ST7735_Screen *self,
	struct ST7735_ScreenAdaptorInterface *adaptor
);

#endif

