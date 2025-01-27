/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include "tileset.h"
#include <SDL3/SDL.h>

#define PIECE_WIDTH 4
#define PIECE_HEIGHT 4

enum PieceId {
    /*
     * RED
     *
     * ######
     * ######
     *    ######
     *    ######
     */
    PIECE_Z,
    /*
     * ORANGE
     *
     *     ###
     *     ###
     *     ###
     * #######
     * #######
     */
    PIECE_L,
    /*
     * YELLOW
     *
     * #######
     * #######
     * #######
     * #######
     */
    PIECE_O,
    /*
     * GREEN
     *
     *    ######
     *    ######
     * ######
     * ######
     */
    PIECE_S,
    /*
     * BLUE
     *
     * ###
     * ###
     * ###
     * ########
     * ########
     */
    PIECE_J,
    /*
     * CYAN
     *
     * #########
     * #########
     */
    PIECE_I,
    /*
     * PURPLE
     *
     *    ###
     *    ###
     * #########
     * #########
     */
    PIECE_T,

    TOTAL_PIECES
};

struct Piece {
    enum PieceId id;
    int prev_rotation;
    int rotation;
    enum TileId tile;
    bool tiles[PIECE_HEIGHT][PIECE_WIDTH];
    enum TileId tilemap[PIECE_HEIGHT][PIECE_WIDTH];
    struct SDL_Point pos;
    struct SDL_Point *kick_offs;
};

struct Piece piece_new(enum PieceId id);
void piece_rotate_90(struct Piece *self);
void piece_rotate_90_cnt(struct Piece *self);
struct SDL_Point piece_get_kick_off(struct Piece *self, int idx);
void piece_render(
    struct Piece *self, struct SDL_Renderer *renderer, struct TileSet *tileset,
    struct SDL_Point pos
);
