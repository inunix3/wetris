/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <SDL3_image/SDL_image.h>
#include <tetris/tileset.h>
#include <tetris/utils.h>

bool tileset_load(
    struct TileSet *tileset, SDL_Renderer *renderer, const char *path
) {
    /* We're starting with 1 because we want to skip TILE_NULL, which doesn't
     * have its own texture */
    for (int i = 0; i < TOTAL_TILES; ++i) {
        SDL_FRect *r = &tileset->rects[i];

        r->x = (float)i * TILE_WIDTH;
        r->y = 0;
        r->w = TILE_WIDTH;
        r->h = TILE_HEIGHT;
    }

    tileset->texture = IMG_LoadTexture(renderer, path);

    if (!tileset->texture) {
        log_error("cannot load tileset '%s': %s", path, SDL_GetError());

        return false;
    }

    return true;
}

void tileset_deinit(struct TileSet *tileset) {
    if (!tileset) {
        return;
    }

    SDL_DestroyTexture(tileset->texture);
    tileset->texture = NULL;
}

void tileset_render_tile(
    struct TileSet *tileset, SDL_Renderer *renderer, enum TileId id, int x,
    int y
) {
    if (id == TILE_NULL) {
        return;
    }

    /* Skip TILE_NULL */
    --id;

    SDL_FRect texture_portion = tileset->rects[id];
    SDL_FRect screen_dest;

    screen_dest.x = (float)x;
    screen_dest.y = (float)y;
    screen_dest.w = TILE_WIDTH;
    screen_dest.h = TILE_HEIGHT;

    SDL_RenderTexture(
        renderer, tileset->texture, &texture_portion, &screen_dest
    );
}
