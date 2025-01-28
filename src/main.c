/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/game.h>
#include <wetris/utils.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>

/* SDL3 leaks too much, probably because of video drivers or something. */
const char *__asan_default_options() { return "detect_leaks=false"; }

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    struct Game *game = game_alloc();

    if (!game_init(game)) {
        log_error("initialization failed");

        return EXIT_FAILURE;
    }

    game_run(game);

    game_deinit(game);
    game_free(game);
    game = NULL;

    return EXIT_SUCCESS;
}
