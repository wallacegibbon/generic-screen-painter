#include "painter.h"
#include "point_iterator.h"
#include <stddef.h>
#include <assert.h>

typedef void *(*PainterInterfaceDefaultFallbackHandler)();
void *PainterInterface_default_fallback_handler() { return NULL; }

void Painter_fill_fallback(
	struct PainterInterface *screen, struct Point p1, struct Point p2, int color
) {
	struct RectPointIterator point_iterator;
	struct Point p;

	RectPointIterator_initialize(&point_iterator, p1, p2);

	while (RectPointIterator_next(&point_iterator, &p))
		screen->draw_point(screen, p, color);
}

void Painter_clear_fallback(struct PainterInterface *screen, int color) {
	struct Point p1, p2;

	Point_initialize(&p1, 0, 0);
	screen->size(screen, &p2);
	Painter_fill_fallback(screen, p1, p2, color);
}

/// The place-holder for interface methods
void PainterInterface_initialize_zero(struct PainterInterface *self) {
	PainterInterfaceDefaultFallbackHandler *fp;
	int size;

	fp = (PainterInterfaceDefaultFallbackHandler *) self;
	size = sizeof(struct PainterInterface) / sizeof(*fp);
	while (size--)
		*fp++ = PainterInterface_default_fallback_handler;
}

void PainterInterface_initialize(struct PainterInterface *self) {
	PainterInterface_initialize_zero(self);
	self->clear = (PainterClear) Painter_clear_fallback;
	self->fill = (PainterFill) Painter_fill_fallback;
}

void Painter_draw_point(struct Painter *self, struct Point p, int color) {
	self->screen->draw_point(self->screen, p, color);
}

void Painter_size(struct Painter *self, struct Point *p) {
	self->screen->size(self->screen, p);
}

void Painter_fill(
	struct Painter *self, struct Point p1, struct Point p2, int color
) {
	self->screen->fill(self->screen, p1, p2, color);
}

void Painter_clear(struct Painter *self, int color) {
	self->screen->clear(self->screen, color);
}

void Painter_flush(struct Painter *self) {
	self->screen->flush(self->screen);
}

void Painter_draw_line(
	struct Painter *self, struct Point p1, struct Point p2, int color
) {
	struct LinePointIterator point_iterator;
	struct Point p;

	LinePointIterator_initialize(&point_iterator, p1, p2);

	while (LinePointIterator_next(&point_iterator, &p))
		Painter_draw_point(self, p, color);
}

void Painter_draw_rectangle(
	struct Painter *self, struct Point p1, struct Point p2, int color
) {
	struct Point tmp;

	Point_initialize(&tmp, p2.x, p1.y);
	Painter_draw_line(self, p1, tmp, color);
	Painter_draw_line(self, tmp, p2, color);

	Point_initialize(&tmp, p1.x, p2.y);
	Painter_draw_line(self, p2, tmp, color);
	Painter_draw_line(self, tmp, p1, color);
}

void Painter_draw_circle(
	struct Painter *self, struct Point p, int radius, int color
) {
	struct CirclePointIterator point_iterator;
	struct Point buffer[8];
	int i;

	CirclePointIterator_initialize(&point_iterator, p, radius);

	while (CirclePointIterator_next(&point_iterator, buffer))
		for (i = 0; i < 8; i++)
			Painter_draw_point(self, buffer[i], color);
}

