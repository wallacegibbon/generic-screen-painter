#ifndef __SSD1306_CH32V_I2C_H
#define __SSD1306_CH32V_I2C_H

#include "sc_ssd1306.h"

struct SSD1306_ScreenAdaptorCH32VI2C {
	const struct SSD1306_ScreenAdaptorInterface *adaptor;
	uint8_t address;
};

void SSD1306_ScreenAdaptorCH32VI2C_initialize(
	struct SSD1306_ScreenAdaptorCH32VI2C *self,
	int address
);

#endif

