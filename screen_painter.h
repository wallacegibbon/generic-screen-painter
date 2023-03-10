#ifndef __SCREEN_PAINTER_H
#define __SCREEN_PAINTER_H

#include "screen_common.h"

typedef void (*DrawingBoardDrawPoint)(void *screen, struct Point p, int color);
typedef void (*DrawingBoardFill)(void *screen, struct Point p1, struct Point p2, int color);
typedef void (*DrawingBoardClear)(void *screen, int color);
typedef void (*DrawingBoardSize)(void *screen, struct Point *p);
typedef void (*DrawingBoardFlush)(void *screen);

/// Screens who implement the Painter interface should put
/// the DrawingBoardInterface at the start of the struct.
struct DrawingBoardInterface {
	/// methods that have to exist
	DrawingBoardDrawPoint draw_point;
	DrawingBoardSize size;

	/// methods with default implementation
	DrawingBoardClear clear;
	DrawingBoardFill fill;

	/// methods that is optional
	DrawingBoardFlush flush;
};

struct Painter {
	struct DrawingBoardInterface *screen;
};

void DrawingBoardInterface_initialize(struct DrawingBoardInterface *self);

/// Core functionalities
static inline void Painter_draw_point(
	struct Painter *self, struct Point p, int color
) {
	self->screen->draw_point(self->screen, p, color);
}

static inline void Painter_size(struct Painter *self, struct Point *p) {
	self->screen->size(self->screen, p);
}

static inline void Painter_fill(
	struct Painter *self, struct Point p1, struct Point p2, int color
) {
	self->screen->fill(self->screen, p1, p2, color);
}

static inline void Painter_clear(struct Painter *self, int color) {
	self->screen->clear(self->screen, color);
}

static inline void Painter_flush(struct Painter *self) {
	self->screen->flush(self->screen);
}

/// Draw methods
void Painter_draw_line(
	struct Painter *self, struct Point p1, struct Point p2, int color
);

void Painter_draw_rectangle(
	struct Painter *self, struct Point p1, struct Point p2, int color
);

void Painter_draw_circle(
	struct Painter *self, struct Point p, int r, int color
);

#endif

