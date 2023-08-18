#ifndef __POINT_ITERATOR_H
#define __POINT_ITERATOR_H

#include "sc_common.h"

typedef int (*PointIteratorNext)(void *iterator, struct Point *result);

struct PointIteratorInterface {
	PointIteratorNext next;
};

/// `self` can point to `LinePointIterator`, `RectPointIterator`, ...
static inline int point_iterator_next(void *self, struct Point *result) {
	return ((struct PointIteratorInterface *)self)->next(self, result);
}

struct LinePointIterator {
	struct PointIteratorInterface iterator;
	struct Point destination;
	struct Point cursor;
	struct Point step;
	struct Point delta;
	struct Point acc;
	int distance;
	int count;
};

struct RectPointIterator {
	struct PointIteratorInterface iterator;
	struct Point p1;
	struct Point p2;
	struct Point cursor;
};

struct CirclePointIterator {
	struct PointIteratorInterface iterator;
	struct Point center;
	int radius;
	int px;
	int py;
};

void line_p_iterator_initialize(struct LinePointIterator *self, struct Point p1, struct Point p2);
void rect_p_terator_initialize(struct RectPointIterator *self, struct Point p1, struct Point p2);
void circle_p_iterator_initialize(struct CirclePointIterator *self, struct Point center, int radius);

#endif
