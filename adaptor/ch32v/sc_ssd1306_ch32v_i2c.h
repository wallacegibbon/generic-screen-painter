#ifndef __SSD1306_CH32V_I2C_H
#define __SSD1306_CH32V_I2C_H

#include "sc_adaptor.h"

struct ssd1306_adaptor_ch32v_i2c {
	struct sc_adaptor_i2c_i *adaptor;
	unsigned char address;
};

int ssd1306_adaptor_ch32v_i2c_init(struct ssd1306_adaptor_ch32v_i2c *self, int address);

#endif
