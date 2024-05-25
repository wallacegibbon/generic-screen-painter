#ifndef __SC_ADAPTOR_I2C_ESP32_H
#define __SC_ADAPTOR_I2C_ESP32_H

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sc_ssd1306.h"

struct sc_adaptor_i2c_esp32 {
	struct ssd1306_adaptor_i *adaptor;
	i2c_cmd_handle_t cmd_handle;
	i2c_port_t i2c_num;
	int address;
};

int sc_adaptor_i2c_esp32_init(struct sc_adaptor_i2c_esp32 *self, int address, i2c_port_t i2c_num);

#endif
