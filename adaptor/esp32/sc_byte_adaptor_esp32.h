#ifndef __SC_BYTE_ADAPTOR_ESP32_H
#define __SC_BYTE_ADAPTOR_ESP32_H

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "sc_adaptor.h"
#include "sc_ssd1306.h"

struct sc_byte_adaptor_esp32 {
	struct sc_byte_adaptor_i *adaptor;
	i2c_cmd_handle_t cmd_handle;
	i2c_port_t i2c_num;
	int address;
};

int sc_byte_adaptor_esp32_init(struct sc_byte_adaptor_esp32 *self, int address, i2c_port_t i2c_num);

#endif
