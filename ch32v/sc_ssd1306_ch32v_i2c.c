#include "sc_ssd1306_ch32v_i2c.h"
#include "sc_common.h"
#include "sc_ch32v_i2c.h"

void ssd1306_adaptor_ch32v_i2c_start_transmit(struct SSD1306_ScreenAdaptorCH32VI2C *self);
void ssd1306_adaptor_ch32v_i2c_stop_transmit(struct SSD1306_ScreenAdaptorCH32VI2C *self);
void ssd1306_adaptor_ch32v_i2c_write_byte(struct SSD1306_ScreenAdaptorCH32VI2C *self, uint8_t data);

static const struct SSD1306_ScreenAdaptorInterface adaptor_vtable = {
	.start_transmit = (SSD1306_ScreenAdaptorStartTransmit) ssd1306_adaptor_ch32v_i2c_start_transmit,
	.stop_transmit = (SSD1306_ScreenAdaptorStopTransmit) ssd1306_adaptor_ch32v_i2c_stop_transmit,
	.write_byte = (SSD1306_ScreenAdaptorWriteByte) ssd1306_adaptor_ch32v_i2c_write_byte
};

void ssd1306_adaptor_ch32v_i2c_start_transmit(struct SSD1306_ScreenAdaptorCH32VI2C *self) {
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
	I2C_GenerateSTART(I2C1, ENABLE);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, self->address, I2C_Direction_Transmitter);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void ssd1306_adaptor_ch32v_i2c_stop_transmit(struct SSD1306_ScreenAdaptorCH32VI2C *self) {
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void ssd1306_adaptor_ch32v_i2c_write_byte(struct SSD1306_ScreenAdaptorCH32VI2C *self, uint8_t data) {
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);
	I2C_SendData(I2C1, data);
}

void ssd1306_adaptor_ch32v_i2c_initialize(struct SSD1306_ScreenAdaptorCH32VI2C *self, int address) {
	GPIO_InitTypeDef gpio_init = { 0 };
	I2C_InitTypeDef i2c_init = { 0 };

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/// When Remap is set, I2C pins will become PB8 and PB9.
	//GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

	gpio_init.GPIO_Pin = GPIO_Pin_6;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	gpio_init.GPIO_Pin = GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_init);

	i2c_init.I2C_ClockSpeed = 400000;
	i2c_init.I2C_Mode = I2C_Mode_I2C;
	i2c_init.I2C_DutyCycle = I2C_DutyCycle_16_9;
	i2c_init.I2C_OwnAddress1 = 0x11;
	i2c_init.I2C_Ack = I2C_Ack_Enable;
	i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &i2c_init);

	I2C_Cmd(I2C1, ENABLE);

	//I2C_AcknowledgeConfig(I2C1, ENABLE);

	self->adaptor = &adaptor_vtable;
	self->address = address << 1;
}

