#include "sc_painter.h"
#include "sc_ascii_font.h"
#include "sc_point_iterator.h"
#include <stdint.h>

static int dawing_board_fill_fallback(void *screen, struct point p1, struct point p2, unsigned long color)
{
	struct rect_point_iter point_iterator;
	struct point p;

	if (rect_p_iter_init(&point_iterator, p1, p2))
		return 1;
	while (!point_iter_next(&point_iterator, &p)) {
		if ((*(struct drawing_i **)screen)->draw_point(screen, p, color))
			return 2;
	}

	return 0;
}

int painter_draw_point(struct painter *self, struct point p, unsigned long color)
{
	drawing_draw_point_fn_t fn;

	fn = (*self->drawing_board)->draw_point;
	if (fn)
		return fn(self->drawing_board, p, color);
	else
		return -2;
}

int painter_size(struct painter *self, struct point *p)
{
	drawing_size_fn_t fn;

	fn = (*self->drawing_board)->size;
	if (fn)
		return fn(self->drawing_board, p);
	else
		return -2;
}

int painter_fill(struct painter *self, struct point p1, struct point p2, unsigned long color)
{
	drawing_fill_fn_t fn;

	fn = (*self->drawing_board)->fill;
	if (fn)
		return fn(self->drawing_board, p1, p2, color);
	else
		return dawing_board_fill_fallback(self->drawing_board, p1, p2, color);
}

int painter_fill_whole(struct painter *self, unsigned long color)
{
	struct point p1, p2;

	if (point_init(&p1, 0, 0))
		return 1;
	if (painter_size(self, &p2))
		return 2;
	if (painter_fill(self, p1, p2, color))
		return 3;

	return 0;
}

int painter_clear(struct painter *self, unsigned long color)
{
	drawing_clear_fn_t fn;

	fn = (*self->drawing_board)->clear;
	if (fn)
		return fn(self->drawing_board, color);
	else
		return painter_fill_whole(self, color);
}

int painter_flush(struct painter *self)
{
	drawing_flush_fn_t fn;

	fn = (*self->drawing_board)->flush;
	if (fn)
		return fn(self->drawing_board);
	else
		return 0;
}

int painter_draw_line(struct painter *self, struct point p1, struct point p2, unsigned long color)
{
	struct line_point_iter point_iterator;
	struct point p;

	if (line_p_iter_init(&point_iterator, p1, p2))
		return 1;
	while (!point_iter_next(&point_iterator, &p)) {
		if (painter_draw_point(self, p, color))
			return 2;
	}

	return 0;
}

int painter_draw_rectangle(struct painter *self, struct point p1, struct point p2, unsigned long color)
{
	struct point tmp;

	point_init(&tmp, p2.x, p1.y);
	painter_draw_line(self, p1, tmp, color);
	painter_draw_line(self, tmp, p2, color);

	point_init(&tmp, p1.x, p2.y);
	painter_draw_line(self, p2, tmp, color);
	painter_draw_line(self, tmp, p1, color);
	return 0;
}

int painter_draw_circle(struct painter *self, struct point p, int radius, unsigned long color)
{
	struct circle_point_iter point_iterator;
	struct point buffer[8];
	int i;

	if (circle_p_iter_init(&point_iterator, p, radius))
		return 1;
	while (!point_iter_next(&point_iterator, buffer)) {
		for (i = 0; i < 8; i++) {
			if (painter_draw_point(self, buffer[i], color))
				return 2;
		}
	}
	return 0;
}

int painter_draw_bezier(struct painter *self, struct point start, struct point end, struct point control, unsigned long color)
{
	struct bezier1_point_iter point_iterator;
	struct point p;

#ifdef BEZIER_DEBUG
	painter_draw_line(self, start, control, GRAY_24bit);
	painter_draw_line(self, control, end, GRAY_24bit);
#endif

	if (bezier1_p_iter_init(&point_iterator, start, end, control))
		return 1;
	while (!point_iter_next(&point_iterator, &p)) {
		if (painter_draw_point(self, p, color))
			return 2;
	}

	return 0;
}

int text_painter_init(struct text_painter *self, struct painter *painter)
{
	self->painter = painter;
	return 0;
}

static void draw_char_byte(struct text_painter *self, unsigned char byte, struct point pos)
{
	struct point p;

	int i, c;
	for (i = 0; i < 8; i++) {
		c = byte & 0x80 ? self->color.foreground : self->color.background;
		point_init(&p, pos.x + i, pos.y);
		painter_draw_point(self->painter, p, c);
		byte <<= 1;
	}
}

static int draw_char_16(struct text_painter *self, int idx, struct point pos)
{
	const unsigned char *buffer;
	struct point p;
	int i;

	buffer = ascii_1608;
	for (i = 0; i < 16; i++) {
		point_init(&p, pos.x, pos.y + i);
		draw_char_byte(self, buffer[idx * 16 + i], p);
	}
	return 1;
}

static int draw_char_32(struct text_painter *self, int idx, struct point pos)
{
	const uint16_t *buffer;
	struct point p;
	int i, t;

	buffer = (const uint16_t *)ascii_3216;
	for (i = 0; i < 32; i++) {
		t = buffer[idx * 32 + i];
		point_init(&p, pos.x, pos.y + i);
		draw_char_byte(self, t & 0xFF, p);
		point_init(&p, pos.x + 8, pos.y + i);
		draw_char_byte(self, t >> 8, p);
	}
	return 1;
}

int text_draw_char(struct text_painter *self, char ch, int size, struct point p)
{
	int idx = ch - ' ';
	if (size == 32)
		return draw_char_32(self, idx, p);
	if (size == 16)
		return draw_char_16(self, idx, p);

	return 0;
}

int text_draw_string(struct text_painter *self, char *str, int size)
{
	int padding, char_width, c, cnt;
	struct point p;

	/// You can make the padding an argument if you want.
	padding = 0;
	char_width = size / 2;

	point_init(&p, self->pos.x, self->pos.y);

	for (c = *str, cnt = 0; c; p.x += char_width + padding, c = *++str)
		cnt += text_draw_char(self, c, size, p);

	return cnt;
}
