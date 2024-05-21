#ifndef __SC_SDLV2_H
#define __SC_SDLV2_H

#include "sc_common.h"
#include "sc_painter.h"
#include <SDL.h>

struct sdlv2_screen {
	struct drawing_i *drawing_board;
	struct point size;
	SDL_Renderer *renderer;
};

int sdlv2_init(struct sdlv2_screen *self, SDL_Renderer *surface, int w, int h);

#endif
