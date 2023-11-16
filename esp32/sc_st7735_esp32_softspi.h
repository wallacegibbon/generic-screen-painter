#ifndef __ST7735_ESP32_SOFTSPI_H
#define __ST7735_ESP32_SOFTSPI_H

#include "sc_st7735.h"

struct st7735_adaptor_esp32_soft_spi {
	const struct st7735_adaptor_i *adaptor;
	uint8_t mosi_pin;
	uint8_t sclk_pin;
	uint8_t cs_pin;
	uint8_t rst_pin;
	uint8_t dc_pin;
};

void st7735_adaptor_esp32_soft_spi_initialize(struct st7735_adaptor_esp32_soft_spi *self, uint8_t mosi_pin, uint8_t sclk_pin, uint8_t cs_pin, uint8_t rst_pin, uint8_t dc_pin);

#endif
