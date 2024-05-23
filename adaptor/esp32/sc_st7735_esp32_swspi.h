#ifndef __ST7735_ESP32_SOFTSPI_H
#define __ST7735_ESP32_SOFTSPI_H

#include "sc_st7735.h"

struct st7735_adaptor_esp32_soft_spi {
	struct st7735_adaptor_i *adaptor;
	int mosi_pin;
	int sclk_pin;
	int cs_pin;
	int rst_pin;
	int dc_pin;
};

int st7735_adaptor_esp32_soft_spi_init(struct st7735_adaptor_esp32_soft_spi *self,
				       int mosi_pin, int sclk_pin, int cs_pin, int rst_pin, int dc_pin);

#endif
