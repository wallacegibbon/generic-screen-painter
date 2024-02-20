#ifndef __SSD1306_CH32V_I2C_H
#define __SSD1306_CH32V_I2C_H

#include "sc_ssd1306.h"

struct ssd1306_adaptor_ch32v_i2c {
	struct ssd1306_adaptor_i *adaptor;
	uint8_t address;
};

void ssd1306_adaptor_ch32v_i2c_initialize(struct ssd1306_adaptor_ch32v_i2c *self, int address);

#endif
