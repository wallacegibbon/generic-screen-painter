#include "sc_point_iterator.h"
#include <stdio.h>

int line_p_iter_next(struct line_point_iter *self, struct point *result);
int rect_p_iter_next(struct rect_point_iter *self, struct point *result);
int circle_p_iter_next(struct circle_point_iter *self, struct point *buffer);

static struct point_iter_i line_p_interface = {
	.next = (point_iter_next_fn)line_p_iter_next,
};

void line_p_iter_init(struct line_point_iter *self, struct point p1, struct point p2)
{
	self->iterator = &line_p_interface;
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

int line_p_iter_next(struct line_point_iter *self, struct point *result)
{
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

static struct point_iter_i rect_p_interface = {
	.next = (point_iter_next_fn)rect_p_iter_next,
};

void rect_p_iter_init(struct rect_point_iter *self, struct point p1, struct point p2)
{
	self->iterator = &rect_p_interface;
	point_init(&self->p1, MIN(p1.x, p2.x), MIN(p1.y, p2.y));
	point_init(&self->p2, MAX(p1.x, p2.x), MAX(p1.y, p2.y));
	self->cursor = self->p1;
}

int rect_p_iter_next(struct rect_point_iter *self, struct point *result)
{
	*result = self->cursor;
	self->cursor.x++;
	if (self->cursor.x > self->p2.x + 1) {
		if (self->cursor.y == self->p2.y)
			return 0;
		self->cursor.x = 0;
		self->cursor.y++;
	}
	return 1;
}

static struct point_iter_i circle_p_interface = {
	.next = (point_iter_next_fn)circle_p_iter_next,
};

void circle_p_iter_init(struct circle_point_iter *self, struct point center, int radius)
{
	self->iterator = &circle_p_interface;
	self->center = center;
	self->radius = radius;
	self->px = radius;
	self->py = 0;
}

int circle_p_iter_next(struct circle_point_iter *self, struct point *buffer)
{
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

int bezier1_p_iter_next(struct bezier1_point_iter *self, struct point *result);

static struct point_iter_i bezier1_p_interface = {
	.next = (point_iter_next_fn)bezier1_p_iter_next,
};

void bezier1_p_iter_init(struct bezier1_point_iter *self, struct point start, struct point end, struct point control)
{
	self->iterator = &bezier1_p_interface;
	self->start = start;
	self->end = end;
	self->control = control;
	self->percent = 0;
	self->step = 0.001;
};

static inline int value_by_percent(int n1, int n2, float percent)
{
	return n1 + ((n2 - n1) * percent);
}

int bezier1_p_iter_next(struct bezier1_point_iter *self, struct point *result)
{
	int xa, xb, ya, yb;
	float percent;

	if (self->percent >= 1)
		return 0;

	percent = self->percent;

	xa = value_by_percent(self->start.x, self->control.x, percent);
	ya = value_by_percent(self->start.y, self->control.y, percent);
	xb = value_by_percent(self->control.x, self->end.x, percent);
	yb = value_by_percent(self->control.y, self->end.y, percent);

	result->x = value_by_percent(xa, xb, percent);
	result->y = value_by_percent(ya, yb, percent);

	self->percent += self->step;

	return 1;
}
