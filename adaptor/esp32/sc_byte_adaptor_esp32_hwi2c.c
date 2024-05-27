#include "sc_byte_adaptor_esp32_hwi2c.h"
#include "esp_log.h"

static int start_transmit(struct sc_byte_adaptor_esp32_hwi2c *self);
static int stop_transmit(struct sc_byte_adaptor_esp32_hwi2c *self);
static int write_byte(struct sc_byte_adaptor_esp32_hwi2c *self, int data);

static struct sc_byte_adaptor_i adaptor_interface = {
	.start_transmit = (sc_adaptor_start_transmit_fn_t)start_transmit,
	.stop_transmit = (sc_adaptor_stop_transmit_fn_t)stop_transmit,
	.write_byte = (sc_adaptor_write_byte_fn_t)write_byte,
};

static int start_transmit(struct sc_byte_adaptor_esp32_hwi2c *self)
{
	self->cmd_handle = i2c_cmd_link_create();
	if (i2c_master_start(self->cmd_handle))
		return 1;
	if (i2c_master_write_byte(self->cmd_handle, self->address | I2C_MASTER_WRITE, true))
		return 2;
	return 0;
}

static int stop_transmit(struct sc_byte_adaptor_esp32_hwi2c *self)
{
	if (i2c_master_stop(self->cmd_handle))
		return 1;
	if (i2c_master_cmd_begin(self->i2c_num, self->cmd_handle, 10 / portTICK_PERIOD_MS))
		return 2;
	i2c_cmd_link_delete(self->cmd_handle);
	return 0;
}

static int write_byte(struct sc_byte_adaptor_esp32_hwi2c *self, int data)
{
	if (i2c_master_write_byte(self->cmd_handle, data, true))
		return 1;
	return 0;
}

int sc_byte_adaptor_esp32_i2c_init(struct sc_byte_adaptor_esp32_hwi2c *self, int address, i2c_port_t i2c_num)
{
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

	if (i2c_param_config(i2c_num, &config))
		return 1;
	if (i2c_driver_install(i2c_num, I2C_MODE_MASTER, 0, 0, 0))
		return 2;

	return 0;
}
