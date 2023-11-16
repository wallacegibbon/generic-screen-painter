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
	struct drawing_i **drawing_board;
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
int painter_draw_char(struct painter *self, char ch, struct point pos, int size, struct color_pair color);
int painter_draw_string(struct painter *self, char *str, struct point pos, int size, struct color_pair color);

#endif
