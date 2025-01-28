/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/ui.h>

#include <wetris/game.h>

#define OUTER_SPACING 16

void ui_init(struct UiState *self, struct Game *game) {
    const struct SDL_Color WHITE = {0xff, 0xff, 0xff, 0xff};
    const struct SDL_Color LIGHT_GRAY = {0x30, 0x30, 0x30, 0xff};

    struct Text *stats = &self->texts[TEXT_STATS];
    struct Text *paused = &self->texts[TEXT_PAUSED];
    struct Text *game_over = &self->texts[TEXT_GAME_OVER];
    struct Text *press_space = &self->texts[TEXT_PRESS_SPACE];
    struct Text *retry_or_quit = &self->texts[TEXT_RETRY_OR_QUIT];
    struct Text *copyright = &self->texts[TEXT_COPYRIGHT];

    self->game = game;

    text_init(
        stats, &game->font_store.fonts[FONT_BIG], TEXT_ALIGN_RIGHT, WHITE
    );

    ui_set_stats(self, 0, 1);

    text_init(
        paused, &game->font_store.fonts[FONT_LARGE], TEXT_ALIGN_LEFT, WHITE
    );

    text_set(&self->texts[TEXT_PAUSED], "PAUSED");

    paused->rect.x = OUTER_SPACING;
    paused->rect.y = game->height - OUTER_SPACING - paused->font->height;
    paused->show = false;

    text_init(
        &self->texts[TEXT_GAME_OVER], &game->font_store.fonts[FONT_LARGE],
        TEXT_ALIGN_LEFT, WHITE
    );

    text_set(&self->texts[TEXT_GAME_OVER], "GAME OVER");

    game_over->rect.x = OUTER_SPACING;
    game_over->rect.y = game->height - OUTER_SPACING - game_over->rect.h;
    game_over->show = false;

    text_init(
        press_space, &game->font_store.fonts[FONT_BIG], TEXT_ALIGN_LEFT, WHITE
    );

    text_set(press_space, "PRESS SPACE TO START");

    press_space->rect.x = game->width / 2 - press_space->rect.w / 2;
    press_space->rect.y = game->height - OUTER_SPACING - press_space->rect.h;
    press_space->show = true;

    text_init(
        retry_or_quit, &game->font_store.fonts[FONT_SMALL], TEXT_ALIGN_LEFT,
        WHITE
    );

    text_set(retry_or_quit, "Press SPACE to retry or ESCAPE to quit");

    retry_or_quit->rect.x = OUTER_SPACING;
    retry_or_quit->rect.y =
        game->height - OUTER_SPACING + game_over->font->line_spacing + 3;
    retry_or_quit->show = false;

    text_init(
        copyright, &game->font_store.fonts[FONT_SMALL], TEXT_ALIGN_RIGHT,
        LIGHT_GRAY
    );

    text_set(
        copyright, "From Czechia with love\nCoded in C using SDL3\n(and its "
                   "extensions image, ttf and mixer)"
    );

    copyright->rect.x = game->width - OUTER_SPACING - copyright->rect.w;
    copyright->rect.y = game->height - OUTER_SPACING - copyright->rect.h;
    copyright->fg = LIGHT_GRAY;
    copyright->show = true;

    self->next_piece_pos.x = self->game->width - TETRION_PADDING_RIGHT / 2 -
                             (PIECE_WIDTH * TILE_WIDTH) / 2;
    self->next_piece_pos.y = 16 + TILE_HEIGHT + self->texts[TEXT_STATS].rect.h;

    ui_set_next_piece(self, &game->tetrion.next_piece);
}

void ui_deinit(struct UiState *self) {
    if (!self) {
        return;
    }

    for (int i = 0; i < TOTAL_TEXTS; ++i) {
        text_deinit(&self->texts[i]);
    }
}

void ui_set_stats(struct UiState *self, int score, int level) {
    struct Text *stats = &self->texts[TEXT_STATS];

    text_set(stats, "SCORE\n%d\n\nLEVEL\n%d", score, level);

    stats->rect.x =
        self->game->width - TETRION_PADDING_RIGHT / 2 - stats->rect.w / 2;
    stats->rect.y = OUTER_SPACING;
}

void ui_set_next_piece(struct UiState *self, const struct Piece *piece) {
    self->next_piece = *piece;
}

void ui_show_text(struct UiState *self, enum TextId id) {
    self->texts[id].show = true;
}

void ui_hide_text(struct UiState *self, enum TextId id) {
    self->texts[id].show = false;
}

void ui_render(struct UiState *self) {
    for (int i = 0; i < TOTAL_TEXTS; ++i) {
        text_render(&self->texts[i]);
    }

    piece_render(
        &self->next_piece, self->game->renderer, &self->game->tileset,
        self->next_piece_pos
    );
}
