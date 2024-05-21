#include "sc_st7789_ch32v_fsmc.h"
#include "ch32v30x_fsmc.h"
#include "ch32v30x_gpio.h"
#include "sc_st7789.h"

static int write_data_16(struct st7789_adaptor_ch32v_fsmc *self, int data);
static int write_data(struct st7789_adaptor_ch32v_fsmc *self, int data);
static int write_cmd(struct st7789_adaptor_ch32v_fsmc *self, int cmd);

static struct st7789_adaptor_i adaptor_interface = {
	.write_data_16 = (st7789_adaptor_write_data_16_fn_t)write_data_16,
	.write_data = (st7789_adaptor_write_data_fn_t)write_data,
	.write_cmd = (st7789_adaptor_write_cmd_fn_t)write_cmd,
};

static int write_data_16(struct st7789_adaptor_ch32v_fsmc *self, int data)
{
	*(volatile uint8_t *)ST7789_LCD_DATA = (uint8_t)(data >> 8);
	*(volatile uint8_t *)ST7789_LCD_DATA = (uint8_t)data;
	return 0;
}

static int write_data(struct st7789_adaptor_ch32v_fsmc *self, int data)
{
	return *(volatile uint8_t *)ST7789_LCD_DATA = data;
}

static int write_cmd(struct st7789_adaptor_ch32v_fsmc *self, int cmd)
{
	return *(volatile uint8_t *)ST7789_LCD_CMD = cmd;
}

int st7789_adaptor_ch32v_fsmc_init(struct st7789_adaptor_ch32v_fsmc *self)
{
	GPIO_InitTypeDef gpio_init = {0};
	FSMC_NORSRAMInitTypeDef fsmc_init = {0};
	FSMC_NORSRAMTimingInitTypeDef read_write_timing = {0};
	FSMC_NORSRAMTimingInitTypeDef write_timing = {0};

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
	gpio_init.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpio_init);

	/// PE7  -- FSMC_D4
	/// PE8  -- FSMC_D5
	/// PE9  -- FSMC_D6
	/// PE10 -- FSMC_D7
	gpio_init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &gpio_init);

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

	fsmc_init.FSMC_Bank = FSMC_Bank1_NORSRAM1;

	fsmc_init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	fsmc_init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	fsmc_init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
	fsmc_init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	fsmc_init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	fsmc_init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	fsmc_init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	fsmc_init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	fsmc_init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	fsmc_init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	fsmc_init.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	fsmc_init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	fsmc_init.FSMC_ReadWriteTimingStruct = &read_write_timing;
	fsmc_init.FSMC_WriteTimingStruct = &write_timing;
	FSMC_NORSRAMInit(&fsmc_init);
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

	self->adaptor = &adaptor_interface;
	return 0;
}
