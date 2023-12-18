#ifndef __SC_COMMON_H
#define __SC_COMMON_H

#include <stdint.h>

struct point {
	int x, y;
};

static inline void point_initialize(struct point *self, int x, int y) {
	self->x = x;
	self->y = y;
}

struct color_pair {
	uint32_t foreground;
	uint32_t background;
};

static inline void color_pair_initialize(struct color_pair *self, uint32_t f, uint32_t b) {
	self->foreground = f;
	self->background = b;
}

void delay(int milliseconds);

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? b : a)
#define ABS(a) (a < 0 ? -a : a)
#define UNIT_VAL(a) (a < 0 ? -1 : 1)
#define SQUARE(a) (a * a)

#endif
