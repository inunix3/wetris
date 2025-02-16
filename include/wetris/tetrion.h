/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include "input.h"
#include "piece.h"
#include "sfx_store.h"
#include "tileset.h"
#include "timer.h"
#include "ui.h"

#include <SDL3/SDL.h>

#include <stdbool.h>

#define FAST_FALL_INTERVAL 50     /* ms */
#define DEFAULT_FALL_INTERVAL 350 /* ms */
#define MIN_FALL_INTERVAL 50      /* ms */
#define MAX_FALL_INTERVAL 320     /* ms */
#define SCORE_SPEED_RATE 30       /* ms */
#define SPEED_UP_RATE 5
#define TETRION_HEIGHT 21 /* the corner is also counted */
#define TETRION_WIDTH 12  /* corners are counted too */
#define MAX_SCORE_PER_LEVEL 100

#define SCORE_ROW_DELETED 10
#define SCORE_MOVE 1
#define SCORE_LANDED 2

enum TetrionState {
    TETRION_STATE_NOT_STARTED,
    TETRION_STATE_NORMAL,
    TETRION_STATE_UPDATING_ROWS,
    TETRION_STATE_GAME_OVER,
};

struct Tetrion {
    struct SDL_Rect rect;
    enum TileId **tilemap;
    struct SfxStore *sfx_store;
    struct UiState *ui;

    int score;
    struct Piece piece;
    struct Piece next_piece;
    uint64_t saved_fall_interval;
    bool lock_saved_fall_interval;
    struct Timer ticker;
    enum TetrionState state;
    bool dropped;
    int level;
};

bool tetrion_init(
    struct Tetrion *self, struct SfxStore *sfx_store, struct UiState *ui, int x,
    int y, int w, int h
);
void tetrion_reset(struct Tetrion *self);
void tetrion_deinit(struct Tetrion *self);
void tetrion_update(struct Tetrion *self);
void tetrion_handle_input(struct Tetrion *self, struct InputState *input);
void tetrion_handle_pause(struct Tetrion *self);
void tetrion_handle_resume(struct Tetrion *self);
void tetrion_render(
    struct Tetrion *self, SDL_Renderer *renderer, struct TileSet *tileset
);
