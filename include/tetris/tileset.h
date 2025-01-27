/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

enum TileId {
    TILE_NULL,
    TILE_WALL,
    TILE_BACKGROUND,
    TILE_FINAL_POS,
    TILE_RED,
    TILE_ORANGE,
    TILE_YELLOW,
    TILE_GREEN,
    TILE_BLUE,
    TILE_CYAN,
    TILE_PURPLE,
    TILE_WHITE,

    TOTAL_TILES,
};

static inline bool tile_is_passable(enum TileId self) {
    return self >= TILE_NULL && self <= TILE_FINAL_POS && self != TILE_WALL;
}

static inline bool tile_is_block(enum TileId self) {
    return self >= TILE_RED && self <= TILE_WHITE;
}

struct TileSet {
    SDL_FRect rects[TOTAL_TILES - 1];
    SDL_Texture *texture;
};

bool tileset_load(
    struct TileSet *tileset, SDL_Renderer *renderer, const char *path
);
void tileset_deinit(struct TileSet *tileset);
void tileset_render_tile(
    struct TileSet *tileset, SDL_Renderer *renderer, enum TileId id, int x,
    int y
);
