#include "esp_log.h"
#include "sc_ssd1306_esp32_i2c.h"

static void start_transmit(struct ssd1306_adaptor_esp32_i2c *self);
static void stop_transmit(struct ssd1306_adaptor_esp32_i2c *self);
static void write_byte(struct ssd1306_adaptor_esp32_i2c *self, uint8_t data);

static const struct ssd1306_adaptor_i adaptor_interface = {
	.start_transmit = (ssd1306_adaptor_start_transmit_fn)start_transmit,
	.stop_transmit = (ssd1306_adaptor_stop_transmit_fn)stop_transmit,
	.write_byte = (ssd1306_adaptor_write_byte_fn)write_byte,
};

static void start_transmit(struct ssd1306_adaptor_esp32_i2c *self) {
	self->cmd_handle = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(self->cmd_handle));
	ESP_ERROR_CHECK(i2c_master_write_byte(self->cmd_handle, self->address | I2C_MASTER_WRITE, true));
}

static void stop_transmit(struct ssd1306_adaptor_esp32_i2c *self) {
	ESP_ERROR_CHECK(i2c_master_stop(self->cmd_handle));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(self->i2c_num, self->cmd_handle, 10 / portTICK_PERIOD_MS));
	i2c_cmd_link_delete(self->cmd_handle);
}

static void write_byte(struct ssd1306_adaptor_esp32_i2c *self, uint8_t data) {
	ESP_ERROR_CHECK(i2c_master_write_byte(self->cmd_handle, data, true));
}

void ssd1306_adaptor_esp32_i2c_initialize(struct ssd1306_adaptor_esp32_i2c *self, int address, i2c_port_t i2c_num) {
	i2c_config_t config;

	self->adaptor = &adaptor_interface;
	self->address = address << 1;
	self->i2c_num = i2c_num;

	config.mode = I2C_MODE_MASTER;
	config.sda_io_num = GPIO_NUM_41;
	config.scl_io_num = GPIO_NUM_40;
	config.sda_pullup_en = GPIO_PULLUP_ENABLE;
	config.scl_pullup_en = GPIO_PULLUP_ENABLE;
	config.master.clk_speed = 400000;
	config.clk_flags = 0;

	ESP_ERROR_CHECK(i2c_param_config(i2c_num, &config));
	ESP_ERROR_CHECK(i2c_driver_install(i2c_num, I2C_MODE_MASTER, 0, 0, 0));
}
