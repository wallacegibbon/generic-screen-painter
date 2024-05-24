#ifndef __ST7735_ESP32_SWSPI_H
#define __ST7735_ESP32_SWSPI_H

#include "sc_adaptor.h"

struct st7735_adaptor_esp32_swspi {
	struct sc_adaptor_i *adaptor;
	int mosi_pin;
	int sclk_pin;
	int cs_pin;
	int rst_pin;
	int dc_pin;
};

int st7735_adaptor_esp32_swspi_init(struct st7735_adaptor_esp32_swspi *self,
				    int mosi_pin, int sclk_pin, int cs_pin, int rst_pin, int dc_pin);

#endif
