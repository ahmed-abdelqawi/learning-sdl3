// SLD3 includes.
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// other includes.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// flags for SDL.
#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Learning SDL3"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

bool game_init_sdl(struct Game *game);
void game_run(struct Game *game);

void housekeeping(struct Game *game);

int main()
{
    bool exit_status = EXIT_FAILURE;

    struct Game game = {NULL, NULL};

    if (game_init_sdl(&game))
    {
        game_run(&game);

        exit_status = EXIT_SUCCESS;
    }

    housekeeping(&game);
    return exit_status;
}

// ==== initialize SLD ====
bool game_init_sdl(struct Game *game)
{
    // SDL, didn't initialize.
    if (!SDL_Init(SDL_FLAGS))
    {
        fprintf(stderr, "Error! Initializing SDL3: %s\n", SDL_GetError());
        return false;
    }

    // creating an SDL window.
    game->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (game->window == NULL)
    {
        fprintf(stderr, "Error! Creating a Window: %s\n", SDL_GetError());
        return false;
    }

    // creating a renderer.
    game->renderer = SDL_CreateRenderer(game->window, NULL);
    if (game->renderer == NULL)
    {
        fprintf(stderr, "Error! Creating a Renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// ==== The Game Itself ====
void game_run(struct Game *game)
{
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);

    SDL_RenderClear(game->renderer);
    SDL_RenderPresent(game->renderer);

    SDL_Delay(5000);
}

// ==== Cleaning up ====
void housekeeping(struct Game *game)
{
    if (game->window)
    { // not sure if I can pass a NULL to this :)
        SDL_DestroyWindow(game->window);
        game->window = NULL;
    }

    if (game->renderer)
    { // not sure if I can pass a NULL to this :)
        SDL_DestroyRenderer(game->renderer);
        game->renderer = NULL;
    }

    SDL_Quit();
}