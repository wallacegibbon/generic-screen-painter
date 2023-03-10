#include "screen_painter.h"
#include "point_iterator.h"
#include <stddef.h>
#include <assert.h>

void DrawingBoard_fill_fallback(
	void *screen, struct Point p1, struct Point p2, int color
) {
	struct RectPointIterator point_iterator;
	struct Point p;

	RectPointIterator_initialize(&point_iterator, p1, p2);

	while (PointIterator_next(&point_iterator, &p))
		(*(struct DrawingBoardInterface **) screen)
			->draw_point(screen, p, color);
}

void DrawingBoard_clear_fallback(void *screen, int color) {
	struct Point p1, p2;

	Point_initialize(&p1, 0, 0);
	(*(struct DrawingBoardInterface **) screen)->size(screen, &p2);
	DrawingBoard_fill_fallback(screen, p1, p2, color);
}

void Painter_draw_point(
	struct Painter *self, struct Point p, int color
) {
	DrawingBoardDrawPoint fn;
	fn = (*self->drawing_board)->draw_point;
	assert(fn);
	if (fn)
		fn(self->drawing_board, p, color);
}

void Painter_size(struct Painter *self, struct Point *p) {
	DrawingBoardSize fn;
	fn = (*self->drawing_board)->size;
	assert(fn);
	if (fn)
		fn(self->drawing_board, p);
}

void Painter_fill(
	struct Painter *self, struct Point p1, struct Point p2, int color
) {
	DrawingBoardFill fn;
	fn = (*self->drawing_board)->fill;
	if (fn)
		fn(self->drawing_board, p1, p2, color);
	else
		DrawingBoard_fill_fallback(self->drawing_board, p1, p2, color);
}

void Painter_clear(struct Painter *self, int color) {
	DrawingBoardClear fn;
	fn = (*self->drawing_board)->clear;
	if (fn)
		fn(self->drawing_board, color);
	else
		DrawingBoard_clear_fallback(self->drawing_board, color);
}

void Painter_flush(struct Painter *self) {
	DrawingBoardFlush fn;
	fn = (*self->drawing_board)->flush;
	if (fn)
		fn(self->drawing_board);
}

void Painter_draw_line(
	struct Painter *self, struct Point p1, struct Point p2, int color
) {
	struct LinePointIterator point_iterator;
	struct Point p;

	LinePointIterator_initialize(&point_iterator, p1, p2);

	while (PointIterator_next(&point_iterator, &p))
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

	while (PointIterator_next(&point_iterator, buffer))
		for (i = 0; i < 8; i++)
			Painter_draw_point(self, buffer[i], color);
}

