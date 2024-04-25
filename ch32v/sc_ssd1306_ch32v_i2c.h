#ifndef __SSD1306_CH32V_I2C_H
#define __SSD1306_CH32V_I2C_H

#include "sc_ssd1306.h"

struct ssd1306_adaptor_ch32v_i2c {
	struct ssd1306_adaptor_i *adaptor;
	unsigned char address;
};

int ssd1306_adaptor_ch32v_i2c_init(struct ssd1306_adaptor_ch32v_i2c *self, int address);

#endif
