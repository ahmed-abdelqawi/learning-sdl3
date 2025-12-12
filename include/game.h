#ifndef GAME_H
#define GAME_H

// SLD3 includes.
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h> // for image stuff.

// other includes.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// flags for SDL.
#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Learning SDL3"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Event event;
    bool is_running;
};

bool game_construct(struct Game **game);
bool game_init_sdl(struct Game *game);

void game_call_events(struct Game *game);

bool game_load_media(struct Game *game);
void game_draw(struct Game *game);
void game_render_color(struct Game *game);

void game_run(struct Game *game);

void housekeeping(struct Game **game);

#endif // GAME_H