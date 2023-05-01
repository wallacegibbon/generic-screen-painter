#include "sc_st7789_ch32v_fsmc.h"
#include "sc_common.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_fsmc.h"

void ST7789_ScreenAdaptorCH32VFSMC_write_data_16(struct ST7789_ScreenAdaptorCH32VFSMC *self, uint16_t data);
void ST7789_ScreenAdaptorCH32VFSMC_write_data(struct ST7789_ScreenAdaptorCH32VFSMC *self, uint8_t data);
void ST7789_ScreenAdaptorCH32VFSMC_write_cmd(struct ST7789_ScreenAdaptorCH32VFSMC *self, uint8_t cmd);

static const struct ST7789_ScreenAdaptorInterface adaptor_vtable = {
	.write_data_16 = (ST7789_ScreenAdaptorWriteData16)
		ST7789_ScreenAdaptorCH32VFSMC_write_data_16,

	.write_data = (ST7789_ScreenAdaptorWriteData)
		ST7789_ScreenAdaptorCH32VFSMC_write_data,

	.write_cmd = (ST7789_ScreenAdaptorWriteCmd)
		ST7789_ScreenAdaptorCH32VFSMC_write_cmd
};

void ST7789_ScreenAdaptorCH32VFSMC_write_data_16(
	struct ST7789_ScreenAdaptorCH32VFSMC *self, uint16_t data
) {
	ST7789_LCD_DATA = (uint8_t) (data >> 8);
	ST7789_LCD_DATA = (uint8_t) data;
}

void ST7789_ScreenAdaptorCH32VFSMC_write_data(
	struct ST7789_ScreenAdaptorCH32VFSMC *self, uint8_t data
) {
	ST7789_LCD_DATA = data;
}

void ST7789_ScreenAdaptorCH32VFSMC_write_cmd(
	struct ST7789_ScreenAdaptorCH32VFSMC *self, uint8_t cmd
) {
	ST7789_LCD_CMD = cmd;
}

void ST7789_ScreenAdaptorCH32VFSMC_initialize(
	struct ST7789_ScreenAdaptorCH32VFSMC *self
) {
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure = { 0 };
	FSMC_NORSRAMTimingInitTypeDef read_write_timing = { 0 };
	FSMC_NORSRAMTimingInitTypeDef write_timing = { 0 };

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

	/// PD14 -- FSMC_D0
	/// PD15 -- FSMC_D1
	/// PD0  -- FSMC_D2
	/// PD1  -- FSMC_D3
	/// PD4  -- FSMC_NOE -- LCD_RD
	/// PD5  -- FSMC_NWE -- LCD_WR
	/// PD12 -- FSMC_A17 -- LCD_DC
	/// PD7  -- FSMC_NE1 -- LCD_CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/// PE7  -- FSMC_D4
	/// PE8  -- FSMC_D5
	/// PE9  -- FSMC_D6
	/// PE10 -- FSMC_D7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	read_write_timing.FSMC_AddressSetupTime = 0x01;
	read_write_timing.FSMC_AddressHoldTime = 0x00;
	read_write_timing.FSMC_DataSetupTime = 0x0f;
	read_write_timing.FSMC_BusTurnAroundDuration = 0x00;
	read_write_timing.FSMC_CLKDivision = 0x00;
	read_write_timing.FSMC_DataLatency = 0x00;
	read_write_timing.FSMC_AccessMode = FSMC_AccessMode_A;

	write_timing.FSMC_AddressSetupTime = 0x00;
	write_timing.FSMC_AddressHoldTime = 0x00;
	write_timing.FSMC_DataSetupTime = 0x03;
	write_timing.FSMC_BusTurnAroundDuration = 0x00;
	write_timing.FSMC_CLKDivision = 0x00;
	write_timing.FSMC_DataLatency = 0x00;
	write_timing.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;

	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &read_write_timing;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &write_timing;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

	self->adaptor = &adaptor_vtable;
}

