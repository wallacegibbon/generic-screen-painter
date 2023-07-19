#ifndef __SCREEN_PAINTER_H
#define __SCREEN_PAINTER_H

#include "sc_common.h"

typedef void (*DrawingBoardDrawPoint)(void *screen, struct Point p, int color);
typedef void (*DrawingBoardFill)(void *screen, struct Point p1, struct Point p2, int color);
typedef void (*DrawingBoardClear)(void *screen, int color);
typedef void (*DrawingBoardSize)(void *screen, struct Point *p);
typedef void (*DrawingBoardFlush)(void *screen);

struct DrawingBoardInterface {
	DrawingBoardDrawPoint draw_point;
	DrawingBoardSize size;
	DrawingBoardClear clear;
	DrawingBoardFill fill;
	DrawingBoardFlush flush;
};

struct Painter {
	struct DrawingBoardInterface **drawing_board;
};

/// Core functionalities
void painter_draw_point(struct Painter *self, struct Point p, int color);
void painter_size(struct Painter *self, struct Point *p);
void painter_clear(struct Painter *self, int color);
void painter_flush(struct Painter *self);

/// Draw methods
void painter_draw_line(struct Painter *self, struct Point p1, struct Point p2, int color);
void painter_draw_rectangle(struct Painter *self, struct Point p1, struct Point p2, int color);
void painter_draw_circle(struct Painter *self, struct Point p, int r, int color);
int painter_draw_char(struct Painter *self, char ch, struct Point pos, int size, struct ColorPair color);
int painter_draw_string(struct Painter *self, char *str, struct Point pos, int size, struct ColorPair color);

#endif

