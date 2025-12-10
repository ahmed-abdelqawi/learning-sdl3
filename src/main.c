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
    SDL_Event event;
    bool is_running;
};

bool game_construct(struct Game **game);
bool game_init_sdl(struct Game *game);

void game_call_events(struct Game *game);
void game_draw(struct Game *game);
void game_run(struct Game *game);

void housekeeping(struct Game **game);

int main()
{
    bool exit_status = EXIT_FAILURE;

    struct Game *game = NULL;

    if (game_construct(&game))
    {
        game_run(game);
        exit_status = EXIT_SUCCESS;
    }

    housekeeping(&game);
    return exit_status;
}

// ==== Game Construction ====
bool game_construct(struct Game **game)
{
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL)
    {
        fprintf(stderr, "Error! Constructing the Game.\n");
        return false;
    }

    struct Game *g = *game;

    if (!game_init_sdl(g))
    {
        return false;
    }

    g->is_running = true;

    return true;
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

// ==== Events ====
void game_call_events(struct Game *game)
{
    // Event Loop.
    while (SDL_PollEvent(&game->event))
    {
        switch (game->event.type)
        {
        case SDL_EVENT_QUIT: // checking if we quit the game.
            game->is_running = false;
            break;

        case SDL_EVENT_KEY_DOWN:
            // checking for keyboard events.
            switch (game->event.key.scancode)
            {
            case SDL_SCANCODE_ESCAPE: // if we pressed escape, it will close the window.
                game->is_running = false;
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

// ==== Rendering ====
void game_draw(struct Game *game)
{
    // clearing and setting the renderer.
    SDL_RenderClear(game->renderer);
    SDL_RenderPresent(game->renderer);
}

// ==== The Game Itself ====
void game_run(struct Game *game)
{
    while (game->is_running)
    {
        // event checking.
        game_call_events(game);

        // rendering.
        game_draw(game);

        // for running the game @ 60fps.
        SDL_Delay(1000 / 60);
    }
}

// ==== Cleaning up ====
void housekeeping(struct Game **game)
{
    if (*game != NULL)
    {
        struct Game *g = *game;

        if (g->window)
        { // not sure if I can pass a NULL to this :)
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

        if (g->renderer)
        { // not sure if I can pass a NULL to this :)
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
        }

        SDL_Quit();

        // cleaning the heap && dangling pointers.
        free(g);
        g = NULL;
        *game = NULL;

        // just for me :)
        printf("All Terminated!\n");
    }
}