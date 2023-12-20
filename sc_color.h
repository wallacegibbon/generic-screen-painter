#ifndef __SC_COLOR_H
#define __SC_COLOR_H

#include <stdint.h>

static inline uint16_t color_to_16bit(uint32_t color) {
        return ((color & 0xF80000) >> 8) | ((color & 0x00F800) >> 5) | ((color & 0x0000F8) >> 3);
}

static inline uint8_t color_to_1bit(uint32_t color) {
	return color ? 1 : 0;
}

#define WHITE_24bit 0xFFFFFF
#define BLACK_24bit 0x000000
#define RED_24bit 0xFF0000
#define GREEN_24bit 0x00FF00
#define BLUE_24bit 0x0000FF
#define CYAN_24bit 0x00FFFF
#define MAGENTA_24bit 0xFF00FF
#define YELLOW_24bit 0xFFFF00
#define BROWN_24bit 0xB88800
#define BRRED_24bit 0xF88038 /// maroon
#define GRAY_24bit 0x808480
#define DARKBLUE_24bit 0x003878
#define LIGHTBLUE_24bit 0x78CCE0
#define GRAYBLUE_24bit 0x5088C0
#define LIGHTGREEN_24bit 0x8080FF
#define LGRAY_24bit 0xC0C0C0
#define LGRAYBLUE_24bit 0xA0C888
#define LBBLUE_24bit 0x286090

#endif
