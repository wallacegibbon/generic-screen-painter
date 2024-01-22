#include "sc_point_iterator.h"
#include "sc_sdlv1.h"
#include <stdlib.h>

void sdlv1_draw_point(struct sdlv1_screen *self, struct point p, uint32_t color);
void sdlv1_size(struct sdlv1_screen *self, struct point *p);
void sdlv1_fill(struct sdlv1_screen *self, struct point p1, struct point p2, uint32_t color);

static const struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn)sdlv1_draw_point,
	.size = (drawing_size_fn)sdlv1_size,
	.fill = (drawing_fill_fn)sdlv1_fill,
};

void sdlv1_draw_point(struct sdlv1_screen *self, struct point p, uint32_t color) {
	uint8_t *pixel;
	int bpp;

	bpp = self->surface->format->BytesPerPixel;
	pixel = (uint8_t *)self->surface->pixels + p.y * self->surface->pitch + p.x * bpp;

	// color = SDL_MapRGB(self->surface->format, 0xFF, 0, 0);
	memcpy(pixel, &color, bpp);
}

void sdlv1_size(struct sdlv1_screen *self, struct point *p) {
	point_initialize(p, self->size.x, self->size.y);
}

void sdlv1_fill(struct sdlv1_screen *self, struct point p1, struct point p2, uint32_t color) {
	SDL_Rect rect;
	rect.x = p1.x;
	rect.y = p1.y;
	rect.w = p2.x - p1.x;
	rect.h = p2.y - p1.y;
	SDL_FillRect(self->surface, &rect, color);
}

void sdlv1_initialize(struct sdlv1_screen *self, SDL_Surface *surface, int w, int h) {
	self->drawing_board = &drawing_interface;
	self->surface = surface;
	self->size.x = w;
	self->size.y = h;
}
