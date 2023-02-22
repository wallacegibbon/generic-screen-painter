#ifndef __SSD1306_H
#define __SSD1306_H

#include "painter.h"
#include <stdint.h>

typedef void (*SSD1306_ScreenAdaptorStartTransmit)(void *adaptor);
typedef void (*SSD1306_ScreenAdaptorStopTransmit)(void *adaptor);
typedef void (*SSD1306_ScreenAdaptorWriteByte)(void *adaptor, uint8_t data);

struct SSD1306_ScreenAdaptorInterface {
	SSD1306_ScreenAdaptorStartTransmit start_transmit;
	SSD1306_ScreenAdaptorStopTransmit stop_transmit;
	SSD1306_ScreenAdaptorWriteByte write_byte;
};

struct SSD1306_Screen {
	/// implement some interface to work with Painter. (vtable)
	struct PainterInterface painter_interface;
	struct SSD1306_ScreenAdaptorInterface *adaptor;
	struct Point size;

	/// just to hold some value since we don't have closure in C.
	int clear_color;

	/// flag to set SSD1306 upside down.
	uint8_t direction;
	uint8_t auto_flush;
	/// the 128x32 mode.
	uint8_t half_mode;
	uint8_t buffer[128][8];
};

void SSD1306_Screen_initialize(
	struct SSD1306_Screen *self,
	struct SSD1306_ScreenAdaptorInterface *adaptor
);

/// SSD1306 specific interfaces
void SSD1306_Screen_fix_32row(struct SSD1306_Screen *self);
void SSD1306_Screen_set_up_down_invert(struct SSD1306_Screen *self);
void SSD1306_Screen_set_brightness(struct SSD1306_Screen *self, uint8_t value);
void SSD1306_Screen_color_reverse(struct SSD1306_Screen *self);
void SSD1306_Screen_display_on(struct SSD1306_Screen *self);
void SSD1306_Screen_display_off(struct SSD1306_Screen *self);

#endif

