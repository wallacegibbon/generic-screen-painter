#ifndef __SSD1306_ESP32_I2C_H
#define __SSD1306_ESP32_I2C_H

#include "ssd1306.h"
#include "driver/i2c.h"
#include "driver/gpio.h"

struct SSD1306_ScreenAdaptorESP32I2C {
	struct SSD1306_ScreenAdaptorInterface adaptor;
	i2c_cmd_handle_t cmd_handle;
	i2c_port_t i2c_num;
	uint8_t address;
};

void SSD1306_ScreenAdaptorESP32I2C_initialize(
	struct SSD1306_ScreenAdaptorESP32I2C *self,
	int address,
	i2c_port_t i2c_num
);

#endif

