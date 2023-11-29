#ifndef __SC_PAINTER_H
#define __SC_PAINTER_H

#include "sc_common.h"

typedef void (*drawing_draw_point_fn)(void *screen, struct point p, int color);
typedef void (*drawing_fill_fn)(void *screen, struct point p1, struct point p2, int color);
typedef void (*drawing_clear_fn)(void *screen, int color);
typedef void (*drawing_size_fn)(void *screen, struct point *p);
typedef void (*drawing_flush_fn)(void *screen);

struct drawing_i {
	drawing_draw_point_fn draw_point;
	drawing_size_fn size;
	drawing_clear_fn clear;
	drawing_fill_fn fill;
	drawing_flush_fn flush;
};

struct painter {
	const struct drawing_i **drawing_board;
};

struct text_painter {
	struct painter *painter;
	struct point pos;
	struct color_pair color;
};

/// Core functionalities
void painter_draw_point(struct painter *self, struct point p, int color);
void painter_size(struct painter *self, struct point *p);
void painter_clear(struct painter *self, int color);
void painter_flush(struct painter *self);

/// Draw methods
void painter_draw_line(struct painter *self, struct point p1, struct point p2, int color);
void painter_draw_rectangle(struct painter *self, struct point p1, struct point p2, int color);
void painter_draw_circle(struct painter *self, struct point p, int r, int color);

void text_painter_initialize(struct text_painter *self, struct painter *painter);
int text_draw_char(struct text_painter *self, char ch, int size, struct point p);
int text_draw_string(struct text_painter *self, char *str, int size);

#endif
