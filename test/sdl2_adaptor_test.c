#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "sc_color.h"
#include "sc_common.h"
#include "sc_painter.h"
#include "sc_sdlv2.h"
#include <SDL.h>
#include <assert.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320

void fancy_display(struct sc_painter *painter)
{
	static int current_cnt = 0, step = 1;
	struct point p;
	struct point size;
	uint32_t color;
	int i;

	sc_painter_size(painter, &size);
	point_init(&p, size.x / 2, size.y / 2);

	for (i = 0; i < 31; i++) {
		color = (ABS(current_cnt - i) < 3) ? BLACK_24bit : CYAN_24bit;
		sc_painter_draw_circle(painter, p, i, color);
	}

	sc_painter_flush(painter);

	if (current_cnt == 31)
		step = -1;
	else if (current_cnt == 0)
		step = 1;

	current_cnt += step;

	SDL_Delay(10);
}

void graphic_play(struct sc_painter *painter)
{
	struct point p1, p2, p3, size;
	struct sc_text_painter text_painter;

	sc_painter_clear(painter, BLACK_24bit);

	/// text drawing
	sc_text_painter_init(&text_painter, painter);

	color_pair_init(&text_painter.color, RED_24bit, BLACK_24bit);
	point_init(&text_painter.pos, 0, 0);

	sc_text_draw_string(&text_painter, "1.5 Programming!", 32);

	color_pair_init(&text_painter.color, GREEN_24bit, BLACK_24bit);
	point_init(&text_painter.pos, 0, 32);

	sc_text_draw_string(&text_painter, "1.5 Programming!", 16);

	sc_painter_size(painter, &size);

	point_init(&p1, size.x / 2 - 50, size.y / 2 - 20);
	point_init(&p2, size.x / 2 + 50, size.y / 2 + 20);
	sc_painter_draw_rectangle(painter, p1, p2, YELLOW_24bit);

	point_init(&p1, size.x / 2 - 50, size.y / 2 - 20);
	sc_painter_draw_circle(painter, p1, 5, MAGENTA_24bit);

	point_init(&p1, 10, size.y / 2 - 20);
	point_init(&p2, 10, size.y / 2 + 20);
	sc_painter_draw_line(painter, p1, p2, WHITE_24bit);

	point_init(&p1, 60, 60);
	point_init(&p2, 10, size.y - 60);
	point_init(&p3, size.x, -60);

	sc_painter_draw_bezier(painter, p1, p2, p3, BLUE_24bit);

	sc_painter_flush(painter);
}

int loop(SDL_Renderer *surface);

int main(int argc, char **argv)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	struct sdlv2_screen screen;
	struct sc_painter painter;
	int r;

	(void)argc;
	(void)argv;

	window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	assert(window != NULL);

	renderer = SDL_CreateRenderer(window, -1, 0);
	assert(renderer != NULL);

	sdlv2_init(&screen, renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	assert(!sc_painter_init(&painter, (struct drawing_i **)&screen));

	graphic_play(&painter);

	while (!loop(renderer))
		fancy_display(&painter);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

int expected_fps = 60;

int loop(SDL_Renderer *renderer)
{
	static unsigned long prev_ticks;
	unsigned long curr_ticks, frame_delay, frame_time;
	SDL_Event ev;

	curr_ticks = SDL_GetTicks();
	if (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT)
			return 1;
	}

	frame_delay = 1000 / expected_fps;
	frame_time = curr_ticks - prev_ticks;
	if (frame_time < frame_delay)
		SDL_Delay(frame_delay - frame_time);

	prev_ticks = curr_ticks;

	SDL_RenderPresent(renderer);

	return 0;
}
