#ifndef __SSD1306_CH32V103_I2C_H
#define __SSD1306_CH32V103_I2C_H

#include "ssd1306.h"

struct SSD1306_ScreenAdaptorCH32V103I2C {
	struct SSD1306_ScreenAdaptorInterface adaptor;
	uint8_t address;
};

void SSD1306_ScreenAdaptorCH32V103I2C_initialize(
	struct SSD1306_ScreenAdaptorCH32V103I2C *self,
	int address
);

#endif

