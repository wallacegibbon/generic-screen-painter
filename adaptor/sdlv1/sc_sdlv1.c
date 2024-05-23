#include "sc_sdlv1.h"
#include "sc_painter.h"
#include <stdint.h>
#include <string.h>

int sdlv1_draw_point(struct sdlv1_screen *self, struct point p, unsigned long color);
int sdlv1_size(struct sdlv1_screen *self, struct point *p);
int sdlv1_fill(struct sdlv1_screen *self, struct point p1, struct point p2, unsigned long color);

static struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn_t)sdlv1_draw_point,
	.size = (drawing_size_fn_t)sdlv1_size,
	.fill = (drawing_fill_fn_t)sdlv1_fill,
};

int sdlv1_draw_point(struct sdlv1_screen *self, struct point p, unsigned long color)
{
	uint8_t *pixel;
	int bpp;

	bpp = self->surface->format->BytesPerPixel;
	pixel = (uint8_t *)self->surface->pixels + p.y * self->surface->pitch + p.x * bpp;

	// color = SDL_MapRGB(self->surface->format, 0xFF, 0, 0);
	memcpy(pixel, &color, bpp);
	return 0;
}

int sdlv1_size(struct sdlv1_screen *self, struct point *p)
{
	return point_init(p, self->size.x, self->size.y);
}

int sdlv1_fill(struct sdlv1_screen *self, struct point p1, struct point p2, unsigned long color)
{
	SDL_Rect rect;

	rect.x = p1.x;
	rect.y = p1.y;
	rect.w = p2.x - p1.x;
	rect.h = p2.y - p1.y;

	if (SDL_FillRect(self->surface, &rect, color))
		return 1;

	return 0;
}

int sdlv1_init(struct sdlv1_screen *self, SDL_Surface *surface, int w, int h)
{
	self->drawing_board = &drawing_interface;
	self->surface = surface;
	self->size.x = w;
	self->size.y = h;

	return 0;
}
