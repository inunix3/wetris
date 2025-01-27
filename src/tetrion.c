/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <stdio.h>
#include <tetris/tetrion.h>

#include <tetris/direction.h>
#include <tetris/tileset.h>
#include <tetris/utils.h>

#include <stdlib.h>

static enum TileId **new_tilemap(int w, int h) {
    enum TileId **tilemap = mem_alloc((size_t)h * sizeof(enum TileId *));

    if (!tilemap) {
        return NULL;
    }

    tilemap[0] = mem_alloc((size_t)(w * h) * sizeof(enum TileId));

    if (!tilemap[0]) {
        mem_free(tilemap);

        return NULL;
    }

    for (int i = 1; i < h; ++i) {
        tilemap[i] = tilemap[0] + i * w;
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            tilemap[y][x] = TILE_NULL;
        }
    }

    return tilemap;
}

static void place_tiles(struct Tetrion *self) {
    int w = (int)self->rect.w;
    int h = (int)self->rect.h;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            self->tilemap[y][x] = TILE_BACKGROUND;
        }
    }

    /* Right and left walls */
    for (int y = 0; y < h; ++y) {
        self->tilemap[y][0] = TILE_WALL;
        self->tilemap[y][w - 1] = TILE_WALL;
    }

    /* Bottom */
    for (int x = 0; x < w; ++x) {
        self->tilemap[h - 1][x] = TILE_WALL;
    }
}

static bool piece_fits(struct Tetrion *self) {
    for (int y = 0; y < PIECE_HEIGHT; ++y) {
        int tile_y = self->piece.pos.y + y;

        for (int x = 0; x < PIECE_WIDTH; ++x) {
            if (!self->piece.tiles[y][x]) {
                continue;
            }

            int tile_x = self->piece.pos.x + x;

            if (tile_x < 0 || tile_x >= self->rect.w || tile_y < 0 ||
                tile_y >= self->rect.h) {
                return false;
            }

            enum TileId tile = self->tilemap[tile_y][tile_x];

            if (!tile_is_passable(tile)) {
                return false;
            }
        }
    }

    return true;
}

static bool move_piece(struct Tetrion *self, enum Direction dir) {
    SDL_assert(dir >= DIR_DOWN && dir <= DIR_LEFT);

    struct SDL_Point old_pos = self->piece.pos;

    switch (dir) {
    case DIR_DOWN:
        self->piece.pos.y += 1;

        break;
    case DIR_RIGHT:
        self->piece.pos.x += 1;

        break;
    case DIR_LEFT:
        self->piece.pos.x -= 1;

        break;
    }

    if (!piece_fits(self)) {
        self->piece.pos = old_pos;

        return false;
    }

    sfx_store_play(self->sfx_store, SFX_MOVE);

    return true;
}

static bool try_adjust_piece(struct Tetrion *self) {
    for (int i = 0; i < 5; ++i) {
        struct SDL_Point kick_off = self->piece.kick_offs[i];

        self->piece.pos.x += kick_off.x;
        self->piece.pos.y += kick_off.y;

        if (piece_fits(self)) {
            return true;
        }
    }

    return false;
}

static void rotate_piece_90(struct Tetrion *self) {
    struct Piece old_piece = self->piece;
    piece_rotate_90(&self->piece);

    if (!piece_fits(self) && !try_adjust_piece(self)) {
        self->piece = old_piece;
    } else {
        sfx_store_play(self->sfx_store, SFX_ROTATE);
    }
}

static void rotate_piece_90_cnt(struct Tetrion *self) {
    struct Piece old_piece = self->piece;
    piece_rotate_90_cnt(&self->piece);

    if (!piece_fits(self) && !try_adjust_piece(self)) {
        self->piece = old_piece;
    } else {
        sfx_store_play(self->sfx_store, SFX_ROTATE);
    }
}

static void speed_up_fall(struct Tetrion *self) {
    self->saved_fall_interval = self->ticker.interval;
    self->ticker.interval /= 5;
    timer_restart(&self->ticker);
}

static void slow_down_fall(struct Tetrion *self) {
    self->ticker.interval = self->saved_fall_interval;
    timer_restart(&self->ticker);
}

static struct Piece gen_piece(struct Tetrion *self) {
    enum PieceId id = (enum PieceId)SDL_rand(TOTAL_PIECES);

    struct Piece piece = piece_new(id);
    piece.pos.x = self->rect.x + PIECE_WIDTH / 2;

    return piece;
}

static void put_piece(struct Tetrion *self) {
    for (int y = 0; y < PIECE_HEIGHT; ++y) {
        int tile_y = self->piece.pos.y + y;

        if (tile_y < 0 || tile_y >= self->rect.h) {
            return;
        }

        for (int x = 0; x < PIECE_WIDTH; ++x) {
            int tile_x = self->piece.pos.x + x;

            if (tile_x < 0 || tile_x >= self->rect.w) {
                continue;
            }

            if (self->piece.tiles[y][x]) {
                self->tilemap[tile_y][tile_x] = self->piece.tile;
            }
        }
    }
}

static void delete_row(struct Tetrion *self, int i) {
    SDL_assert(i >= 0);

    for (int y = i; y > 0; --y) {
        for (int x = 1; x < self->rect.w - 1; ++x) {
            self->tilemap[y][x] = self->tilemap[y - 1][x];
            self->tilemap[y - 1][x] = TILE_BACKGROUND;
        }
    }

    sfx_store_play(self->sfx_store, SFX_DELETED_ROW);
}

static void add_score(struct Tetrion *self, int score) {
    int new_score_ratio = (self->score + score) / MAX_SCORE_PER_LEVEL;
    int next_score_ratio = self->score / MAX_SCORE_PER_LEVEL + 1;

    if (self->score > 0 && new_score_ratio >= next_score_ratio) {
        ++self->level;

        sfx_store_play(self->sfx_store, SFX_LEVEL_UP);

        self->ticker.interval -= SPEED_UP_RATE;
        self->ticker.interval = SDL_clamp(
            self->ticker.interval, MIN_FALL_INTERVAL, DEFAULT_FALL_INTERVAL
        );

        self->saved_fall_interval = self->ticker.interval;
    }

    self->score += score;
    ui_set_stats(self->ui, self->score, self->level);
}

static void drop_piece(struct Tetrion *self) {
    for (int i = 0; i < self->rect.h - 1; ++i) {
        if (!move_piece(self, DIR_DOWN)) {
            self->dropped = true;
            timer_restart(&self->ticker);

            break;
        }

        add_score(self, SCORE_MOVE);
    }
}

static void update_rows(struct Tetrion *self) {
    for (int y = 0; y < self->rect.h - 1; ++y) { /* -1 for bottom walls */
        int block_cnt = 0;

        for (int x = 1; x < self->rect.w - 1; ++x) { /* -1 for right wall */
            enum TileId tile = self->tilemap[y][x];

            if (tile_is_block(tile)) {
                ++block_cnt;
            }
        }

        if (block_cnt >= self->rect.w - 2) { /* -2 for walls */
            delete_row(self, y);
            add_score(self, SCORE_ROW_DELETED);
            timer_restart(&self->ticker);

            return;
        }
    }

    self->state = TETRION_STATE_NORMAL;
}

static void do_tick(struct Tetrion *self) {
    if (!piece_fits(self)) {
        self->state = TETRION_STATE_GAME_OVER;

        ui_show_text(self->ui, TEXT_GAME_OVER);
        ui_show_text(self->ui, TEXT_RETRY_OR_QUIT);
        sfx_store_play(self->sfx_store, SFX_GAME_OVER);

        return;
    }

    if (!move_piece(self, DIR_DOWN)) {
        put_piece(self);

        self->state = TETRION_STATE_UPDATING_ROWS;

        self->piece = self->next_piece;
        self->next_piece = gen_piece(self);

        ui_set_next_piece(self->ui, &self->next_piece);

        if (self->dropped) {
            self->dropped = false;
            sfx_store_play(self->sfx_store, SFX_DROP);
        } else {
            sfx_store_play(self->sfx_store, SFX_LANDED);
        }

        add_score(self, SCORE_LANDED);
    } else {
        add_score(self, SCORE_MOVE);
    }

    timer_restart(&self->ticker);
}

static void render_piece_shadow(
    struct Tetrion *self, struct SDL_Renderer *renderer, struct TileSet *tileset
) {
    for (int y = 0; y < PIECE_HEIGHT; ++y) {
        int tile_y = self->piece.pos.y + y;

        for (int x = 0; x < PIECE_WIDTH; ++x) {
            int tile_x = self->piece.pos.x + x;

            if (!self->piece.tiles[y][x]) {
                continue;
            }

            for (int i = tile_y + 1; i < self->rect.h - 1 &&
                                     tile_is_passable(self->tilemap[i][tile_x]);
                 ++i) {
                tileset_render_tile(
                    tileset, renderer, TILE_FINAL_POS,
                    TILE_WIDTH * (self->rect.x + tile_x),
                    TILE_HEIGHT * (self->rect.y + i)
                );
            }
        }
    }
}

bool tetrion_init(
    struct Tetrion *self, struct SfxStore *sfx_store, struct UiState *ui, int x,
    int y, int w, int h
) {
    self->tilemap = new_tilemap((int)w, (int)h);

    if (!self->tilemap) {
        log_error("cannot create a tile map");

        return false;
    }

    self->rect.x = x / TILE_WIDTH;
    self->rect.y = y / TILE_WIDTH;
    self->rect.w = w;
    self->rect.h = h;
    self->sfx_store = sfx_store;
    self->ui = ui;

    place_tiles(self);
    tetrion_reset(self);

    return true;
}

void tetrion_reset(struct Tetrion *self) {
    self->score = 0;
    self->piece = gen_piece(self);
    self->next_piece = gen_piece(self);
    self->saved_fall_interval = DEFAULT_FALL_INTERVAL;
    self->ticker = timer_new(DEFAULT_FALL_INTERVAL);
    self->state = TETRION_STATE_NOT_STARTED;
    self->dropped = false;
    self->level = 1;

    for (int y = 0; y < self->rect.h - 1; ++y) {
        for (int x = 1; x < self->rect.w - 1; ++x) {
            self->tilemap[y][x] = TILE_BACKGROUND;
        }
    }
}

void tetrion_deinit(struct Tetrion *self) {
    if (!self) {
        return;
    }

    if (self->tilemap) {
        mem_free(self->tilemap[0]);
        mem_free(self->tilemap);
        self->tilemap = NULL;
    }
}

void tetrion_update(struct Tetrion *self) {
    switch (self->state) {
    case TETRION_STATE_NOT_STARTED:
        break;
    case TETRION_STATE_NORMAL:
        if (!timer_done(&self->ticker)) {
            break;
        }

        do_tick(self);

        break;
    case TETRION_STATE_UPDATING_ROWS:
        if (!timer_done(&self->ticker)) {
            break;
        }

        update_rows(self);

        break;
    case TETRION_STATE_GAME_OVER:
        break;
    }
}

void tetrion_handle_input(struct Tetrion *self, struct InputState *state) {
    if (self->state == TETRION_STATE_GAME_OVER) {
        if (input_key_released(state, SDL_SCANCODE_SPACE)) {
            tetrion_reset(self);
            self->state = TETRION_STATE_NORMAL;

            ui_set_stats(self->ui, self->score, self->level);
            ui_set_next_piece(self->ui, &self->next_piece);

            ui_hide_text(self->ui, TEXT_GAME_OVER);
            ui_hide_text(self->ui, TEXT_RETRY_OR_QUIT);
        }
    } else if (self->state == TETRION_STATE_NOT_STARTED) {
        if (input_key_released(state, SDL_SCANCODE_SPACE)) {
            self->state = TETRION_STATE_NORMAL;

            ui_hide_text(self->ui, TEXT_PRESS_SPACE);
            sfx_store_play(self->sfx_store, SFX_LEVEL_UP);
        }
    }

    if (self->state == TETRION_STATE_NORMAL ||
        self->state == TETRION_STATE_UPDATING_ROWS) {
        if (input_key_down(state, SDL_SCANCODE_A) ||
            input_key_down(state, SDL_SCANCODE_LEFT)) {
            move_piece(self, DIR_LEFT);
        }

        if (input_key_down(state, SDL_SCANCODE_D) ||
            input_key_down(state, SDL_SCANCODE_RIGHT)) {
            move_piece(self, DIR_RIGHT);
        }

        if (input_key_released(state, SDL_SCANCODE_E)) {
            rotate_piece_90(self);
        }

        if (input_key_released(state, SDL_SCANCODE_Q)) {
            rotate_piece_90_cnt(self);
        }
    }

    if (self->state == TETRION_STATE_NORMAL ||
        self->state == TETRION_STATE_UPDATING_ROWS) {
        if (input_key_pressed(state, SDL_SCANCODE_S) ||
            input_key_pressed(state, SDL_SCANCODE_DOWN)) {
            speed_up_fall(self);
        } else if (input_key_released(state, SDL_SCANCODE_S) ||
                   input_key_released(state, SDL_SCANCODE_DOWN)) {
            slow_down_fall(self);
        }

        if (input_key_pressed(state, SDL_SCANCODE_SPACE)) {
            drop_piece(self);
        }
    }
}

void tetrion_handle_pause(struct Tetrion *self) { slow_down_fall(self); }

void tetrion_render(
    struct Tetrion *self, struct SDL_Renderer *renderer, struct TileSet *tileset
) {
    for (int y = 0; y < (int)self->rect.h; ++y) {
        for (int x = 0; x < (int)self->rect.w; ++x) {
            int tile_x = TILE_WIDTH * (self->rect.x + x);
            int tile_y = TILE_HEIGHT * (self->rect.y + y);

            tileset_render_tile(
                tileset, renderer, self->tilemap[y][x], tile_x, tile_y
            );
        }
    }

    struct SDL_Point piece_pos = {
        TILE_WIDTH * (self->rect.x + self->piece.pos.x),
        TILE_HEIGHT * (self->rect.y + self->piece.pos.y)
    };

    render_piece_shadow(self, renderer, tileset);
    piece_render(&self->piece, renderer, tileset, piece_pos);
}
