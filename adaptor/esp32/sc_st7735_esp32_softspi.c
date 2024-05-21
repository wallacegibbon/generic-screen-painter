#include "sc_st7735_esp32_softspi.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "sc_common.h"

static int write_data_16(struct st7735_adaptor_esp32_soft_spi *self, int data);
static int write_data(struct st7735_adaptor_esp32_soft_spi *self, int data);
static int write_cmd(struct st7735_adaptor_esp32_soft_spi *self, int data);

static struct st7735_adaptor_i adaptor_interface = {
	.write_data_16 = (st7735_adaptor_write_data_16_fn)write_data_16,
	.write_data = (st7735_adaptor_write_data_fn)write_data,
	.write_cmd = (st7735_adaptor_write_cmd_fn)write_cmd,
};

static int write_byte(struct st7735_adaptor_esp32_soft_spi *self, int data)
{
	int i;
	ESP_ERROR_CHECK(gpio_set_level(self->cs_pin, 0));
	for (i = 0; i < 8; i++) {
		ESP_ERROR_CHECK(gpio_set_level(self->sclk_pin, 0));
		ESP_ERROR_CHECK(gpio_set_level(self->mosi_pin, data & 0x80));
		ESP_ERROR_CHECK(gpio_set_level(self->sclk_pin, 1));
		data <<= 1;
	}
	ESP_ERROR_CHECK(gpio_set_level(self->cs_pin, 1));
	return 0;
}

static int write_data_16(struct st7735_adaptor_esp32_soft_spi *self, int data)
{
	ESP_ERROR_CHECK(gpio_set_level(self->dc_pin, 1));
	write_byte(self, data >> 8);
	write_byte(self, data);
	return 0;
}

static int write_data(struct st7735_adaptor_esp32_soft_spi *self, int data)
{
	ESP_ERROR_CHECK(gpio_set_level(self->dc_pin, 1));
	return write_byte(self, data);
}

static int write_cmd(struct st7735_adaptor_esp32_soft_spi *self, int data)
{
	ESP_ERROR_CHECK(gpio_set_level(self->dc_pin, 0));
	return write_byte(self, data);
}

int st7735_adaptor_esp32_soft_spi_init(struct st7735_adaptor_esp32_soft_spi *self,
				       int mosi_pin, int sclk_pin, int cs_pin, int rst_pin, int dc_pin)
{
	self->adaptor = &adaptor_interface;

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
	return 0;
}
