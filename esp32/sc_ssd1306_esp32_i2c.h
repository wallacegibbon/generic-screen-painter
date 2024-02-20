#ifndef __SSD1306_ESP32_I2C_H
#define __SSD1306_ESP32_I2C_H

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sc_ssd1306.h"

struct ssd1306_adaptor_esp32_i2c {
	struct ssd1306_adaptor_i *adaptor;
	i2c_cmd_handle_t cmd_handle;
	i2c_port_t i2c_num;
	uint8_t address;
};

void ssd1306_adaptor_esp32_i2c_initialize(struct ssd1306_adaptor_esp32_i2c *self, int address, i2c_port_t i2c_num);

#endif
