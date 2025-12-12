#include "../include/game.h"

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