#include "../include/game.h"

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

    if (!game_load_media(g))
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

    // setting the window icon.
    SDL_Surface *icon_surf = IMG_Load("images/cIcon.png");
    if (icon_surf && !SDL_SetWindowIcon(game->window, icon_surf))
    {
        fprintf(stderr, "Error! Loading Window Icon: %s\n", SDL_GetError());
        SDL_DestroySurface(icon_surf);
        return false;
    }
    SDL_DestroySurface(icon_surf);

    srand((unsigned int)time(NULL));

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

            case SDL_SCANCODE_SPACE: // when we press space it will change the background color.
                game_render_color(game);
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

// === Texture Media ====
bool game_load_media(struct Game *game)
{
    // loading the background texture from an image.
    game->background = IMG_LoadTexture(game->renderer, "images/background.png");
    if (!game->background)
    {
        fprintf(stderr, "Error! Loading the Background Texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// ==== Rendering ====
void game_draw(struct Game *game)
{
    // clearing the renderer.
    SDL_RenderClear(game->renderer);

    // loading the background.
    SDL_RenderTexture(game->renderer, game->background, NULL, NULL);

    // setting the
    SDL_RenderPresent(game->renderer);
}

void game_render_color(struct Game *game)
{
    uint8_t red = (uint8_t)rand() % 256;
    uint8_t green = (uint8_t)rand() % 256;
    uint8_t blue = (uint8_t)rand() % 256;

    SDL_SetRenderDrawColor(game->renderer, red, green, blue, SDL_ALPHA_OPAQUE);
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

        if (g->background)
        {
            SDL_DestroyTexture(g->background);
            g->background = NULL;
        }

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

        // take SDL offline.
        SDL_Quit();

        // cleaning the heap && dangling pointers.
        free(g);
        g = NULL;
        *game = NULL;

        // just for me :)
        printf("All Terminated!\n");
    }
}