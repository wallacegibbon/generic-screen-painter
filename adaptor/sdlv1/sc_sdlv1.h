#ifndef __SC_SDLV1_H
#define __SC_SDLV1_H

#include "sc_common.h"
#include "sc_painter.h"
#include <SDL.h>

struct sdlv1_screen {
	struct drawing_i *drawing_board;
	struct point size;
	SDL_Surface *surface;
};

int sdlv1_init(struct sdlv1_screen *self, SDL_Surface *surface, int w, int h);

#endif
