#ifndef __SCREEN_COMMON_H
#define __SCREEN_COMMON_H

struct Point { int x, y; };

static inline void Point_initialize(struct Point *self, int x, int y) {
	self->x = x;
	self->y = y;
}

struct ColorPair { int foreground; int background; };

static inline void ColorPair_initialize(struct ColorPair *self, int f, int b) {
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

