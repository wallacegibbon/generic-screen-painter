#include "point_iterator.h"
#include <stdio.h>

void RectPointIterator_initialize(
	struct RectPointIterator *self, struct Point p1, struct Point p2
) {
	Point_initialize(&self->p1, MIN(p1.x, p2.x), MIN(p1.y, p2.y));
	Point_initialize(&self->p2, MAX(p1.x, p2.x), MAX(p1.y, p2.y));
	self->cursor = self->p1;
}

int RectPointIterator_next(
	struct RectPointIterator *self, struct Point *result
) {
	//printf("next rect point {%d,%d}\n", self->cursor.x, self->cursor.y);
	*result = self->cursor;
	self->cursor.x++;
	if (self->cursor.x > self->p2.x + 1) {
		if (self->cursor.y == self->p2.y) return 0;
		self->cursor.x = 0;
		self->cursor.y++;
	}
	return 1;
}

void RectPointIterator_describe(struct RectPointIterator *self) {
	printf(
		"RectPointIterator: p1{%d,%d} p2{%d,%d} cursor{%d,%d}\n",
		self->p1.x,
		self->p1.y,
		self->p2.x,
		self->p2.y,
		self->cursor.x,
		self->cursor.y
	);
}

void LinePointIterator_initialize(
	struct LinePointIterator *self, struct Point p1, struct Point p2
) {
	self->cursor = p1;
	self->destination = p2;
	self->delta.x = p2.x - p1.x;
	self->delta.y = p2.y - p1.y;
	self->step.x = UNIT_VAL(self->delta.x);
	self->step.y = UNIT_VAL(self->delta.y);
	self->delta.x = ABS(self->delta.x);
	self->delta.y = ABS(self->delta.y);
	self->acc.x = 0;
	self->acc.y = 0;
	self->distance = MAX(self->delta.x, self->delta.y);
	self->count = 0;
}

int LinePointIterator_next(
	struct LinePointIterator *self, struct Point *result
) {
	//printf("next line point {%d,%d}\n", self->cursor.x, self->cursor.y);
	*result = self->cursor;
	self->acc.x += self->delta.x;
	if (self->acc.x >= self->distance) {
		self->acc.x -= self->distance;
		self->cursor.x += self->step.x;
	}
	self->acc.y += self->delta.y;
	if (self->acc.y >= self->distance) {
		self->acc.y -= self->distance;
		self->cursor.y += self->step.y;
	}
	return self->count++ < self->distance;
}

void CirclePointIterator_initialize(
	struct CirclePointIterator *self, struct Point center, int radius
) {
	self->center = center;
	self->radius = radius;
	self->px = radius;
	self->py = 0;
}

int CirclePointIterator_next(
	struct CirclePointIterator *self, struct Point *buffer
) {
	if (self->py > self->px)
		return 0;

	buffer[0].x = self->center.x - self->px;
	buffer[0].y = self->center.y - self->py;
	buffer[1].x = self->center.x - self->px;
	buffer[1].y = self->center.y + self->py;
	buffer[2].x = self->center.x + self->px;
	buffer[2].y = self->center.y - self->py;
	buffer[3].x = self->center.x + self->px;
	buffer[3].y = self->center.y + self->py;

	buffer[4].x = self->center.x - self->py;
	buffer[4].y = self->center.y - self->px;
	buffer[5].x = self->center.x - self->py;
	buffer[5].y = self->center.y + self->px;
	buffer[6].x = self->center.x + self->py;
	buffer[6].y = self->center.y - self->px;
	buffer[7].x = self->center.x + self->py;
	buffer[7].y = self->center.y + self->px;

	self->py++;

	if ((SQUARE(self->py) + SQUARE(self->px)) > SQUARE(self->radius))
		self->px--;

	return 1;
}

