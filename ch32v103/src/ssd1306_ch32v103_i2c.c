#include "ssd1306_ch32v103_i2c.h"

void SSD1306_ScreenAdaptorCH32V103I2C_start_transmit(
	struct SSD1306_ScreenAdaptorCH32V103I2C *self
) {
}

void SSD1306_ScreenAdaptorCH32V103I2C_stop_transmit(
	struct SSD1306_ScreenAdaptorCH32V103I2C *self
) {
}

void SSD1306_ScreenAdaptorCH32V103I2C_write_byte(
	struct SSD1306_ScreenAdaptorCH32V103I2C *self, uint8_t data
) {
}

void SSD1306_ScreenAdaptorCH32V103I2C_initialize(
	struct SSD1306_ScreenAdaptorCH32V103I2C *self,
	int address
) {
	self->adaptor.start_transmit = (SSD1306_ScreenAdaptorStartTransmit)
		SSD1306_ScreenAdaptorCH32V103I2C_start_transmit;

	self->adaptor.stop_transmit = (SSD1306_ScreenAdaptorStopTransmit)
		SSD1306_ScreenAdaptorCH32V103I2C_stop_transmit;

	self->adaptor.write_byte = (SSD1306_ScreenAdaptorWriteByte)
		SSD1306_ScreenAdaptorCH32V103I2C_write_byte;

	self->address = address << 1;
}

