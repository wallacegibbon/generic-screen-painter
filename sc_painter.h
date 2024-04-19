#ifndef __SC_PAINTER_H
#define __SC_PAINTER_H

#include "sc_common.h"

typedef void (*drawing_draw_point_fn_t)(void *screen, struct point p, uint32_t color);
typedef void (*drawing_fill_fn_t)(void *screen, struct point p1, struct point p2, uint32_t color);
typedef void (*drawing_clear_fn_t)(void *screen, uint32_t color);
typedef void (*drawing_size_fn_t)(void *screen, struct point *p);
typedef void (*drawing_flush_fn_t)(void *screen);

struct drawing_i {
	drawing_draw_point_fn_t draw_point;
	drawing_size_fn_t size;
	drawing_clear_fn_t clear;
	drawing_fill_fn_t fill;
	drawing_flush_fn_t flush;
};

struct painter {
	struct drawing_i **drawing_board;
};

struct text_painter {
	struct painter *painter;
	struct point pos;
	struct color_pair color;
};

/// Core functionalities
void painter_draw_point(struct painter *self, struct point p, uint32_t color);
void painter_size(struct painter *self, struct point *p);
void painter_clear(struct painter *self, uint32_t color);
void painter_flush(struct painter *self);

/// Draw methods
void painter_draw_line(struct painter *self, struct point p1, struct point p2, uint32_t color);
void painter_draw_rectangle(struct painter *self, struct point p1, struct point p2, uint32_t color);
void painter_draw_circle(struct painter *self, struct point p, int r, uint32_t color);
void painter_draw_bezier(struct painter *self, struct point start, struct point end, struct point control, uint32_t color);

void text_painter_init(struct text_painter *self, struct painter *painter);
int text_draw_char(struct text_painter *self, char ch, int size, struct point p);
int text_draw_string(struct text_painter *self, char *str, int size);

#endif
