#ifndef __SSD1306_STM32F10x_I2C_H
#define __SSD1306_STM32F10x_I2C_H

#include "ssd1306.h"

struct SSD1306_ScreenAdaptorSTM32F10xI2C {
	struct SSD1306_ScreenAdaptorInterface adaptor;
	uint8_t address;
};

void SSD1306_ScreenAdaptorSTM32F10xI2C_initialize(
	struct SSD1306_ScreenAdaptorSTM32F10xI2C *self,
	int address
);

#endif

