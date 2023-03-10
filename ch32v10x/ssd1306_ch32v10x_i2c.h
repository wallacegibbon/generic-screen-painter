#ifndef __SSD1306_CH32V10x_I2C_H
#define __SSD1306_CH32V10x_I2C_H

#include "ssd1306.h"

struct SSD1306_ScreenAdaptorCH32V10xI2C {
	const struct SSD1306_ScreenAdaptorInterface *adaptor;
	uint8_t address;
};

void SSD1306_ScreenAdaptorCH32V10xI2C_initialize(
	struct SSD1306_ScreenAdaptorCH32V10xI2C *self,
	int address
);

#endif

