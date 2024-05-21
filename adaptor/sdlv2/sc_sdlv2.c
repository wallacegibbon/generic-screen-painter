#include "sc_sdlv2.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "sc_painter.h"

int sdlv2_draw_point(struct sdlv2_screen *self, struct point p, unsigned long color);
int sdlv2_size(struct sdlv2_screen *self, struct point *p);
int sdlv2_fill(struct sdlv2_screen *self, struct point p1, struct point p2, unsigned long color);

static struct drawing_i drawing_interface = {
	.draw_point = (drawing_draw_point_fn_t)sdlv2_draw_point,
	.size = (drawing_size_fn_t)sdlv2_size,
	.fill = (drawing_fill_fn_t)sdlv2_fill,
};

int sdlv2_set_color(struct sdlv2_screen *self, unsigned long color)
{
	return SDL_SetRenderDrawColor(self->renderer,
				      (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF,
				      SDL_ALPHA_OPAQUE);
}

int sdlv2_draw_point(struct sdlv2_screen *self, struct point p, unsigned long color)
{
	if (sdlv2_set_color(self, color))
		return 1;
	if (SDL_RenderDrawPoint(self->renderer, p.x, p.y))
		return 2;

	return 0;
}

int sdlv2_size(struct sdlv2_screen *self, struct point *p)
{
	return point_init(p, self->size.x, self->size.y);
}

int sdlv2_fill(struct sdlv2_screen *self, struct point p1, struct point p2, unsigned long color)
{
	SDL_Rect rect;
	rect.x = p1.x;
	rect.y = p1.y;
	rect.w = p2.x - p1.x;
	rect.h = p2.y - p1.y;

	if (sdlv2_set_color(self, color))
		return 1;
	if (SDL_RenderFillRect(self->renderer, &rect))
		return 2;

	return 0;
}

int sdlv2_init(struct sdlv2_screen *self, SDL_Renderer *renderer, int w, int h)
{
	self->drawing_board = &drawing_interface;
	self->renderer = renderer;
	self->size.x = w;
	self->size.y = h;

	return 0;
}
