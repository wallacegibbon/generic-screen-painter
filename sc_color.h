#ifndef __SC_COLOR_H
#define __SC_COLOR_H

static inline unsigned int color_to_16bit(unsigned long color)
{
	return ((color & 0xF80000) >> 8) | ((color & 0x00FC00) >> 5) | ((color & 0x0000F8) >> 3);
}

static inline unsigned int color_to_1bit(unsigned long color)
{
	return color ? 1 : 0;
}

/// These color values are copied from Wikipedia.
#define WHITE_24bit 0xFFFFFF
#define BLACK_24bit 0x000000
#define GRAY_24bit 0x808080
#define LIGHTGRAY_24bit 0xC0C0C0
#define RED_24bit 0xFF0000
#define GREEN_24bit 0x00FF00
#define BLUE_24bit 0x0000FF
#define CYAN_24bit 0x00FFFF
#define MAGENTA_24bit 0xFF00FF
#define YELLOW_24bit 0xFFFF00
#define BROWN_24bit 0x7F3F00
#define MAROON_24bit 0x800000
#define DARKBLUE_24bit 0x00008B
#define LIGHTBLUE_24bit 0xADD8E6
#define LIGHTGREEN_24bit 0x90EE90

#endif
