#ifndef __SC_POINT_ITERATOR_H
#define __SC_POINT_ITERATOR_H

#include "sc_common.h"

typedef int (*point_iter_next_fn)(void *iterator, struct point *result);

struct point_iter_i {
	point_iter_next_fn next;
};

static inline int point_iter_next(void *self, struct point *result) {
	return (*(struct point_iter_i **)self)->next(self, result);
}

struct line_point_iter {
	struct point_iter_i *iterator;
	struct point destination;
	struct point cursor;
	struct point step;
	struct point delta;
	struct point acc;
	uint16_t distance;
	uint16_t count;
};

struct rect_point_iter {
	struct point_iter_i *iterator;
	struct point p1;
	struct point p2;
	struct point cursor;
};

struct circle_point_iter {
	struct point_iter_i *iterator;
	struct point center;
	int radius;
	int px;
	int py;
};

struct bezier1_point_iter {
	struct point_iter_i *iterator;
	struct point start;
	struct point end;
	struct point control;
	float percent;
	float step;
};

void line_p_iter_init(struct line_point_iter *self, struct point p1, struct point p2);
void rect_p_iter_init(struct rect_point_iter *self, struct point p1, struct point p2);
void circle_p_iter_init(struct circle_point_iter *self, struct point center, int radius);
void bezier1_p_iter_init(struct bezier1_point_iter *self, struct point start, struct point end, struct point control);

#endif
