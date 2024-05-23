#ifndef __SC_PAINTER_H
#define __SC_PAINTER_H

#include "sc_common.h"

typedef int (*drawing_draw_point_fn_t)(void *screen, struct point p, unsigned long color);
typedef int (*drawing_fill_fn_t)(void *screen, struct point p1, struct point p2, unsigned long color);
typedef int (*drawing_clear_fn_t)(void *screen, unsigned long color);
typedef int (*drawing_size_fn_t)(void *screen, struct point *p);
typedef int (*drawing_flush_fn_t)(void *screen);

struct drawing_i {
	drawing_draw_point_fn_t draw_point;
	drawing_size_fn_t size;
	drawing_clear_fn_t clear;
	drawing_fill_fn_t fill;
	drawing_flush_fn_t flush;
};

struct sc_painter {
	struct drawing_i **drawing_board;
};

/// Core functionalities
int sc_painter_init(struct sc_painter *self, struct drawing_i **drawing_board);

int sc_painter_draw_point(struct sc_painter *self, struct point p, unsigned long color);

int sc_painter_size(struct sc_painter *self, struct point *p);

int sc_painter_clear(struct sc_painter *self, unsigned long color);

int sc_painter_flush(struct sc_painter *self);

/// Draw methods
int sc_painter_draw_line(struct sc_painter *self, struct point p1, struct point p2, unsigned long color);

int sc_painter_draw_rectangle(struct sc_painter *self, struct point p1, struct point p2, unsigned long color);

int sc_painter_draw_circle(struct sc_painter *self, struct point p, int r, unsigned long color);

int sc_painter_draw_bezier(struct sc_painter *self, struct point start, struct point end,
			   struct point control, unsigned long color);

struct sc_text_painter {
	struct sc_painter *painter;
	struct point pos;
	struct color_pair color;
};

int sc_text_painter_init(struct sc_text_painter *self, struct sc_painter *painter);

int sc_text_draw_char(struct sc_text_painter *self, char ch, int size, struct point p);

int sc_text_draw_string(struct sc_text_painter *self, char *str, int size);

#endif
