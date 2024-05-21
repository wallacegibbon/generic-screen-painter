#ifndef __SC_COMMON_H
#define __SC_COMMON_H

struct point {
	int x, y;
};

static inline int point_init(struct point *self, int x, int y)
{
	self->x = x;
	self->y = y;
	return 0;
}

struct color_pair {
	unsigned long foreground;
	unsigned long background;
};

static inline int color_pair_init(struct color_pair *self, unsigned long f, unsigned long b)
{
	self->foreground = f;
	self->background = b;
	return 0;
}

int delay(int milliseconds);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define ABS(a) ((a) < 0 ? -(a) : (a))
#define UNIT_VAL(a) ((a) < 0 ? -1 : 1)
#define SQUARE(a) ((a) * (a))

#endif
