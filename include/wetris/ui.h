/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

/*
 * NOTE: it looks like immediate gui would fit better, but I have written it
 * already as a retained gui, so I don't want to rewrite again.
 */

#include "piece.h"
#include "text.h"

#include <SDL3/SDL.h>

struct Game;

enum TextId {
    TEXT_STATS,
    TEXT_COPYRIGHT,
    TEXT_PAUSED,
    TEXT_GAME_OVER,
    TEXT_PRESS_SPACE,
    TEXT_RETRY_OR_QUIT,

    TOTAL_TEXTS
};

struct UiState {
    struct Text texts[TOTAL_TEXTS];
    struct Game *game;
    struct Piece next_piece;
    struct SDL_Point next_piece_pos;
};

void ui_init(struct UiState *self, struct Game *game);
void ui_deinit(struct UiState *self);
void ui_set_stats(struct UiState *self, int score, int level);
void ui_set_next_piece(struct UiState *self, const struct Piece *piece);
void ui_show_text(struct UiState *self, enum TextId id);
void ui_hide_text(struct UiState *self, enum TextId id);
void ui_render(struct UiState *self);
