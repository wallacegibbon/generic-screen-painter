#include "ssd1306_ch32v10x_i2c.h"
#include "ch32v10x_i2c.h"
#include "screen_common.h"

/// This driver use I2C1 on PB8(SCL) and PB9(SDA)

void SSD1306_ScreenAdaptorCH32V10xI2C_start_transmit(
	struct SSD1306_ScreenAdaptorCH32V10xI2C *self
) {
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET);
	I2C_GenerateSTART(I2C1, ENABLE);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, self->address, I2C_Direction_Transmitter);

	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

void SSD1306_ScreenAdaptorCH32V10xI2C_stop_transmit(
	struct SSD1306_ScreenAdaptorCH32V10xI2C *self
) {
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void SSD1306_ScreenAdaptorCH32V10xI2C_write_byte(
	struct SSD1306_ScreenAdaptorCH32V10xI2C *self, uint8_t data
) {
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET);
	I2C_SendData(I2C1, data);
}

void SSD1306_ScreenAdaptorCH32V10xI2C_initialize(
	struct SSD1306_ScreenAdaptorCH32V10xI2C *self, int address
) {
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	I2C_InitTypeDef I2C_InitTSturcture = { 0 };

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitTSturcture.I2C_ClockSpeed = 400000;
	I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
	I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
	I2C_InitTSturcture.I2C_OwnAddress1 = 0x11;
	I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
	I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitTSturcture);

	I2C_Cmd(I2C1, ENABLE);

	//I2C_AcknowledgeConfig(I2C1, ENABLE);

	self->adaptor.start_transmit = (SSD1306_ScreenAdaptorStartTransmit)
		SSD1306_ScreenAdaptorCH32V10xI2C_start_transmit;

	self->adaptor.stop_transmit = (SSD1306_ScreenAdaptorStopTransmit)
		SSD1306_ScreenAdaptorCH32V10xI2C_stop_transmit;

	self->adaptor.write_byte = (SSD1306_ScreenAdaptorWriteByte)
		SSD1306_ScreenAdaptorCH32V10xI2C_write_byte;

	self->address = address << 1;
}

