#include "sc_adaptor_esp32_swspi.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "sc_adaptor.h"
#include "sc_common.h"

static int start_transmit(struct sc_adaptor_esp32_swspi *self);
static int stop_transmit(struct sc_adaptor_esp32_swspi *self);
static int write_data(struct sc_adaptor_esp32_swspi *self, int data);
static int write_cmd(struct sc_adaptor_esp32_swspi *self, int data);

static struct sc_adaptor_i adaptor_interface = {
	.start_transmit = (sc_adaptor_start_transmit_fn_t)start_transmit,
	.stop_transmit = (sc_adaptor_stop_transmit_fn_t)stop_transmit,
	.write_data = (sc_adaptor_write_data_fn_t)write_data,
	.write_cmd = (sc_adaptor_write_cmd_fn_t)write_cmd,
};

static int start_transmit(struct sc_adaptor_esp32_swspi *self)
{
	// if (gpio_set_level(self->cs_pin, 0) != ESP_OK)
	if (gpio_set_level(self->cs_pin, 0))
		return 1;

	return 0;
}

static int stop_transmit(struct sc_adaptor_esp32_swspi *self)
{
	if (gpio_set_level(self->cs_pin, 1))
		return 1;

	return 0;
}

static int write_byte(struct sc_adaptor_esp32_swspi *self, int data)
{
	int i;

	for (i = 0; i < 8; i++) {
		if (gpio_set_level(self->sclk_pin, 0))
			return 2;
		if (gpio_set_level(self->mosi_pin, data & 0x80))
			return 2;
		if (gpio_set_level(self->sclk_pin, 1))
			return 2;
		data <<= 1;
	}

	return 0;
}

static int write_data(struct sc_adaptor_esp32_swspi *self, int data)
{
	if (gpio_set_level(self->dc_pin, 1))
		return 0xFF;

	return write_byte(self, data);
}

static int write_cmd(struct sc_adaptor_esp32_swspi *self, int data)
{
	if (gpio_set_level(self->dc_pin, 0))
		return 0xFF;

	return write_byte(self, data);
}

int sc_adaptor_esp32_swspi_init(struct sc_adaptor_esp32_swspi *self,
				int mosi_pin, int sclk_pin, int cs_pin, int rst_pin, int dc_pin)
{
	self->adaptor = &adaptor_interface;

	if (gpio_set_direction(mosi_pin, GPIO_MODE_OUTPUT))
		return 1;
	if (gpio_set_direction(sclk_pin, GPIO_MODE_OUTPUT))
		return 2;
	if (gpio_set_direction(cs_pin, GPIO_MODE_OUTPUT))
		return 3;
	if (gpio_set_direction(rst_pin, GPIO_MODE_OUTPUT))
		return 4;
	if (gpio_set_direction(dc_pin, GPIO_MODE_OUTPUT))
		return 5;

	self->mosi_pin = mosi_pin;
	self->sclk_pin = sclk_pin;
	self->cs_pin = cs_pin;
	self->rst_pin = rst_pin;
	self->dc_pin = dc_pin;

	if (gpio_set_level(rst_pin, 0))
		return 6;

	delay(200);

	if (gpio_set_level(rst_pin, 1))
		return 7;

	delay(20);
	return 0;
}
