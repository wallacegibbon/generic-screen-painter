#include "sc_adaptor_ch32v_hwspi.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_spi.h"
#include "sc_adaptor.h"
#include "sc_common.h"

static int start_transmit(struct sc_adaptor_ch32v_hwspi *self);
static int stop_transmit(struct sc_adaptor_ch32v_hwspi *self);
static int write_data(struct sc_adaptor_ch32v_hwspi *self, int data);
static int write_cmd(struct sc_adaptor_ch32v_hwspi *self, int cmd);

static struct sc_adaptor_i adaptor_interface = {
	.start_transmit = (sc_adaptor_start_transmit_fn_t)start_transmit,
	.stop_transmit = (sc_adaptor_stop_transmit_fn_t)stop_transmit,
	.write_data = (sc_adaptor_write_data_fn_t)write_data,
	.write_cmd = (sc_adaptor_write_cmd_fn_t)write_cmd,
};

int SPI1_write(unsigned char data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI1, data);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
		;

	return 0;
}

static int start_transmit(struct sc_adaptor_ch32v_hwspi *self)
{
	/// CS = 0;
	GPIOA->BCR = GPIO_Pin_4;
	return 0;
}

static int stop_transmit(struct sc_adaptor_ch32v_hwspi *self)
{
	/// CS = 1;
	GPIOA->BSHR = GPIO_Pin_4;
	return 0;
}

static int write_data(struct sc_adaptor_ch32v_hwspi *self, int data)
{
	/// DC = 1;
	GPIOA->BSHR = GPIO_Pin_3;
	/// Setting CS in `start_transmit` and `stop_transmit` is not working
	GPIOA->BCR = GPIO_Pin_4;
	SPI1_write(data);
	GPIOA->BSHR = GPIO_Pin_4;
	return 0;
}

static int write_cmd(struct sc_adaptor_ch32v_hwspi *self, int cmd)
{
	/// DC = 0;
	GPIOA->BCR = GPIO_Pin_3;
	/// Setting CS in `start_transmit` and `stop_transmit` is not working
	GPIOA->BCR = GPIO_Pin_4;
	SPI1_write(cmd);
	GPIOA->BSHR = GPIO_Pin_4;
	return 0;
}

int sc_adaptor_ch32v_hwspi_init(struct sc_adaptor_ch32v_hwspi *self)
{
	GPIO_InitTypeDef gpio_init = {0};
	SPI_InitTypeDef spi_init = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

	gpio_init.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);

	gpio_init.GPIO_Pin = GPIO_Pin_6;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);

	gpio_init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);

	spi_init.SPI_Mode = SPI_Mode_Master;
	spi_init.SPI_Direction = SPI_Direction_1Line_Tx;
	spi_init.SPI_DataSize = SPI_DataSize_8b;
	spi_init.SPI_CPOL = SPI_CPOL_Low;
	spi_init.SPI_CPHA = SPI_CPHA_1Edge;
	spi_init.SPI_NSS = SPI_NSS_Soft;
	spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
	spi_init.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &spi_init);

	SPI_Cmd(SPI1, ENABLE);

	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	delay(100);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	delay(100);

	self->adaptor = &adaptor_interface;
	return 0;
}
