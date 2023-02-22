#include "st7735_esp32_softspi.h"
#include "common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

void ST7735_ScreenAdaptorESP32SoftSPI_write_byte(
	struct ST7735_ScreenAdaptorESP32SoftSPI *self, uint8_t data
) {
	int i;
	ESP_ERROR_CHECK(gpio_set_level(self->cs_pin, 0));
	for (i = 0; i < 8; i++) {
		ESP_ERROR_CHECK(gpio_set_level(self->sclk_pin, 0));
		ESP_ERROR_CHECK(gpio_set_level(self->mosi_pin, data & 0x80));
		ESP_ERROR_CHECK(gpio_set_level(self->sclk_pin, 1));
		data <<= 1;
	}
	ESP_ERROR_CHECK(gpio_set_level(self->cs_pin, 1));
}

void ST7735_ScreenAdaptorESP32SoftSPI_write_data_16(
	struct ST7735_ScreenAdaptorESP32SoftSPI *self, uint16_t data
) {
	ESP_ERROR_CHECK(gpio_set_level(self->dc_pin, 1));
	ST7735_ScreenAdaptorESP32SoftSPI_write_byte(self, data >> 8);
	ST7735_ScreenAdaptorESP32SoftSPI_write_byte(self, data);
}

void ST7735_ScreenAdaptorESP32SoftSPI_write_data(
	struct ST7735_ScreenAdaptorESP32SoftSPI *self, uint8_t data
) {
	ESP_ERROR_CHECK(gpio_set_level(self->dc_pin, 1));
	ST7735_ScreenAdaptorESP32SoftSPI_write_byte(self, data);
}

void ST7735_ScreenAdaptorESP32SoftSPI_write_cmd(
	struct ST7735_ScreenAdaptorESP32SoftSPI *self, uint8_t data
) {
	ESP_ERROR_CHECK(gpio_set_level(self->dc_pin, 0));
	ST7735_ScreenAdaptorESP32SoftSPI_write_byte(self, data);
}

void ST7735_ScreenAdaptorESP32SoftSPI_initialize(
	struct ST7735_ScreenAdaptorESP32SoftSPI *self,
	uint8_t mosi_pin,
	uint8_t sclk_pin,
	uint8_t cs_pin,
	uint8_t rst_pin,
	uint8_t dc_pin
) {
	self->adaptor.write_data_16 = (ST7735_ScreenAdaptorWriteData16)
		ST7735_ScreenAdaptorESP32SoftSPI_write_data_16;

	self->adaptor.write_data = (ST7735_ScreenAdaptorWriteData)
		ST7735_ScreenAdaptorESP32SoftSPI_write_data;

	self->adaptor.write_cmd = (ST7735_ScreenAdaptorWriteCmd)
		ST7735_ScreenAdaptorESP32SoftSPI_write_cmd;

	ESP_ERROR_CHECK(gpio_set_direction(mosi_pin, GPIO_MODE_OUTPUT));
	ESP_ERROR_CHECK(gpio_set_direction(sclk_pin, GPIO_MODE_OUTPUT));
	ESP_ERROR_CHECK(gpio_set_direction(cs_pin, GPIO_MODE_OUTPUT));
	ESP_ERROR_CHECK(gpio_set_direction(rst_pin, GPIO_MODE_OUTPUT));
	ESP_ERROR_CHECK(gpio_set_direction(dc_pin, GPIO_MODE_OUTPUT));

	self->mosi_pin = mosi_pin;
	self->sclk_pin = sclk_pin;
	self->cs_pin = cs_pin;
	self->rst_pin = rst_pin;
	self->dc_pin = dc_pin;

	ESP_ERROR_CHECK(gpio_set_level(rst_pin, 0));
	delay(200);
	ESP_ERROR_CHECK(gpio_set_level(rst_pin, 1));
	delay(20);
}

