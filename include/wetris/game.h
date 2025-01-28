/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include "font_store.h"
#include "input.h"
#include "sfx_store.h"
#include "tetrion.h"
#include "ui.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <stdbool.h>

#define DEFAULT_FPS 60
#define TETRION_PADDING_LEFT (TILE_WIDTH * 3)
#define TETRION_PADDING_RIGHT (TILE_WIDTH * 10)
#define WINDOW_WIDTH                                                           \
    (TETRION_PADDING_LEFT + TETRION_PADDING_RIGHT + TETRION_WIDTH * TILE_WIDTH)
#define WINDOW_HEIGHT (TILE_HEIGHT * (TETRION_HEIGHT + 4))

enum GameState { GAME_RUNNING, GAME_PAUSED, GAME_QUIT };

struct Game {
    int width;
    int height;
    struct SDL_Window *window;
    struct SDL_Renderer *renderer;
    struct SDL_Texture *background;
    struct UiState ui;
    struct TileSet tileset;
    struct SfxStore sfx_store;
    struct FontStore font_store;
    struct InputState input;

    enum GameState state;
    struct Tetrion tetrion;
};

struct Game *game_alloc(void);
void game_free(struct Game *self);
bool game_init(struct Game *self);
void game_deinit(struct Game *self);
void game_run(struct Game *self);
