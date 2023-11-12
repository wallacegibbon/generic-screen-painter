#include "sc_ascii_font.h"
#include "sc_painter.h"
#include "sc_point_iterator.h"
#include <assert.h>
#include <stddef.h>

static void dawing_board_fill_fallback(void *screen, struct Point p1, struct Point p2, int color) {
	struct RectPointIterator point_iterator;
	struct Point p;

	rect_p_terator_initialize(&point_iterator, p1, p2);
	while (point_iterator_next(&point_iterator, &p))
		(*(struct DrawingBoardInterface **)screen)->draw_point(screen, p, color);
}

void painter_draw_point(struct Painter *self, struct Point p, int color) {
	DrawingBoardDrawPoint fn;
	fn = (*self->drawing_board)->draw_point;
	assert(fn);
	if (fn)
		fn(self->drawing_board, p, color);
}

void painter_size(struct Painter *self, struct Point *p) {
	DrawingBoardSize fn;
	fn = (*self->drawing_board)->size;
	assert(fn);
	if (fn)
		fn(self->drawing_board, p);
}

void painter_fill(struct Painter *self, struct Point p1, struct Point p2, int color) {
	DrawingBoardFill fn;
	fn = (*self->drawing_board)->fill;
	if (fn)
		fn(self->drawing_board, p1, p2, color);
	else
		dawing_board_fill_fallback(self->drawing_board, p1, p2, color);
}

void painter_fill_whole(struct Painter *self, int color) {
	struct Point p1, p2;
	point_initialize(&p1, 0, 0);
	painter_size(self, &p2);
	painter_fill(self, p1, p2, color);
}

void painter_clear(struct Painter *self, int color) {
	DrawingBoardClear fn;
	fn = (*self->drawing_board)->clear;
	if (fn)
		fn(self->drawing_board, color);
	else
		painter_fill_whole(self, color);
}

void painter_flush(struct Painter *self) {
	DrawingBoardFlush fn;
	fn = (*self->drawing_board)->flush;
	if (fn)
		fn(self->drawing_board);
}

void painter_draw_line(struct Painter *self, struct Point p1, struct Point p2, int color) {
	struct LinePointIterator point_iterator;
	struct Point p;

	line_p_iterator_initialize(&point_iterator, p1, p2);
	while (point_iterator_next(&point_iterator, &p))
		painter_draw_point(self, p, color);
}

void painter_draw_rectangle(struct Painter *self, struct Point p1, struct Point p2, int color) {
	struct Point tmp;

	point_initialize(&tmp, p2.x, p1.y);
	painter_draw_line(self, p1, tmp, color);
	painter_draw_line(self, tmp, p2, color);

	point_initialize(&tmp, p1.x, p2.y);
	painter_draw_line(self, p2, tmp, color);
	painter_draw_line(self, tmp, p1, color);
}

void painter_draw_circle(struct Painter *self, struct Point p, int radius, int color) {
	struct CirclePointIterator point_iterator;
	struct Point buffer[8];
	int i;

	circle_p_iterator_initialize(&point_iterator, p, radius);
	while (point_iterator_next(&point_iterator, buffer)) {
		for (i = 0; i < 8; i++)
			painter_draw_point(self, buffer[i], color);
	}
}

static int painter_draw_char_byte(struct Painter *self, uint8_t byte, int x, int y, struct ColorPair color) {
	struct Point p;
	int i, c;
	for (i = 0; i < 8; i++) {
		c = byte & 0x80 ? color.foreground : color.background;
		point_initialize(&p, x + i, y);
		painter_draw_point(self, p, c);
		byte <<= 1;
	}
	return 1;
}

static int painter_draw_char_16(struct Painter *self, int idx, struct Point pos, const uint8_t *buffer, struct ColorPair color) {
	int i;

	for (i = 0; i < 16; i++)
		painter_draw_char_byte(self, buffer[idx * 16 + i], pos.x, pos.y + i, color);

	return 1;
}

static int painter_draw_char_32(struct Painter *self, int idx, struct Point pos, const uint16_t *buffer, struct ColorPair color) {
	int i, t;
	for (i = 0; i < 32; i++) {
		t = buffer[idx * 32 + i];
		painter_draw_char_byte(self, t & 0xFF, pos.x, pos.y + i, color);
		painter_draw_char_byte(self, t >> 8, pos.x + 8, pos.y + i, color);
	}
	return 1;
}

int painter_draw_char(struct Painter *self, char ch, struct Point pos, int size, struct ColorPair color) {
	int idx = ch - ' ';
	if (size == 32)
		return painter_draw_char_32(self, idx, pos, (const uint16_t *)ascii_3216, color);
	if (size == 16)
		return painter_draw_char_16(self, idx, pos, ascii_1608, color);

	return 0;
}

int painter_draw_string(struct Painter *self, char *str, struct Point pos, int size, struct ColorPair color) {
	/// You can make the padding an argument if you want.
	int padding = 0;
	int char_width = size / 2;
	int c = *str;
	int cnt = 0;

	for (; c; pos.x += char_width + padding, c = *++str)
		cnt += painter_draw_char(self, c, pos, size, color);

	return cnt;
}
