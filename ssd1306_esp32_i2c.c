#include "ssd1306_esp32_i2c.h"
#include "esp_log.h"

void SSD1306_ScreenAdaptorESP32I2C_start_transmit(
	struct SSD1306_ScreenAdaptorESP32I2C *self
) {
	self->cmd_handle = i2c_cmd_link_create();
	ESP_ERROR_CHECK(i2c_master_start(self->cmd_handle));
	ESP_ERROR_CHECK(i2c_master_write_byte(
		self->cmd_handle, self->address | I2C_MASTER_WRITE, true
	));
}

void SSD1306_ScreenAdaptorESP32I2C_stop_transmit(
	struct SSD1306_ScreenAdaptorESP32I2C *self
) {
	ESP_ERROR_CHECK(i2c_master_stop(self->cmd_handle));
	ESP_ERROR_CHECK(i2c_master_cmd_begin(
		self->i2c_num, self->cmd_handle, 10 / portTICK_PERIOD_MS
	));
	i2c_cmd_link_delete(self->cmd_handle);
}

void SSD1306_ScreenAdaptorESP32I2C_write_byte(
	struct SSD1306_ScreenAdaptorESP32I2C *self, uint8_t data
) {
	ESP_ERROR_CHECK(i2c_master_write_byte(self->cmd_handle, data, true));
}

void SSD1306_ScreenAdaptorESP32I2C_initialize(
	struct SSD1306_ScreenAdaptorESP32I2C *self,
	int address,
	i2c_port_t i2c_num
) {
	i2c_config_t config;

	self->adaptor.start_transmit = (SSD1306_ScreenAdaptorStartTransmit)
		SSD1306_ScreenAdaptorESP32I2C_start_transmit;

	self->adaptor.stop_transmit = (SSD1306_ScreenAdaptorStopTransmit)
		SSD1306_ScreenAdaptorESP32I2C_stop_transmit;

	self->adaptor.write_byte = (SSD1306_ScreenAdaptorWriteByte)
		SSD1306_ScreenAdaptorESP32I2C_write_byte;

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

