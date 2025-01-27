/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <tetris/piece.h>

#include <string.h>

/* clang-format off */
static const bool g_pieces[TOTAL_PIECES][4][PIECE_HEIGHT][PIECE_WIDTH] = {
    {
        /* Z - red */
        {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    {
        /* L - orange */
        {
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
    {
        /* O - yellow */
        {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        }
    },
    {
        /* S - green */
        {
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
    {
        /* J - blue */
        {
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
    {
        /* I - cyan */
        {
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        },
        {
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0}
        }
    },
    {
        /* T - purple */
        {
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
        {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        }
    },
};
/* clang-format on */

/* Kick offsets for J, L, S, T, Z tetrominos */
static struct SDL_Point g_kick_offs[4][5] = {
    // 0 -> 90
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},

    // 90 -> 180
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},

    // 180 -> 270
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}},

    // 270 -> 0
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}
};

/* Kick offsets for J, L, S, T, Z tetrominos */
static struct SDL_Point g_kick_offs_cnt[4][5] = {
    // 90 -> 0
    {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}},

    // 180 -> 90
    {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}},

    // 270 -> 180
    {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}},

    // 0 -> 270
    {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}
};

/* Kick offsets for I tetromino */
static struct SDL_Point g_i_kick_offs[4][5] = {
    // 0 -> 90
    {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},

    // 90 -> 180
    {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}},

    // 180 -> 270
    {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},

    // 270 -> 0
    {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}
};

/* Kick offsets for I tetromino */
static struct SDL_Point g_i_kick_offs_cnt[4][5] = {
    // 90 -> 0
    {{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}},

    // 180 -> 90
    {{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}},

    // 270 -> 180
    {{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}},

    // 0 -> 270
    {{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}
};

static enum TileId piece_tile(enum PieceId id) {
    switch (id) {
    case PIECE_Z:
        return TILE_RED;
    case PIECE_L:
        return TILE_ORANGE;
    case PIECE_O:
        return TILE_YELLOW;
    case PIECE_S:
        return TILE_GREEN;
    case PIECE_J:
        return TILE_BLUE;
    case PIECE_I:
        return TILE_CYAN;
    case PIECE_T:
        return TILE_PURPLE;
    case TOTAL_PIECES:
        /* doesn't make sense, but what else can we do? */
        return TOTAL_TILES;
    }
}

static void update_tilemap(struct Piece *self) {
    memset(self->tilemap, TILE_NULL, sizeof(self->tilemap));

    for (int y = 0; y < PIECE_HEIGHT; ++y) {
        for (int x = 0; x < PIECE_WIDTH; ++x) {
            enum TileId *tile = &self->tilemap[y][x];

            if (self->tiles[y][x]) {
                *tile = self->tile;
            } else {
                *tile = TILE_NULL;
            }
        }
    }
}

struct Piece piece_new(enum PieceId id) {
    struct Piece piece;

    piece.id = id;
    piece.tile = piece_tile(id);
    piece.prev_rotation = 0;
    piece.rotation = 0;
    memcpy(piece.tiles, g_pieces[id][0], sizeof(piece.tiles));
    piece.pos = (SDL_Point){0, 0};
    piece.kick_offs = id == PIECE_I ? &g_i_kick_offs[0][0] : &g_kick_offs[0][0];

    update_tilemap(&piece);

    return piece;
}

void piece_rotate_90(struct Piece *self) {
    self->prev_rotation = self->rotation;

    if (self->rotation == 3) {
        self->rotation = 0;
    } else {
        ++self->rotation;
    }

    if (self->id == PIECE_I) {
        self->kick_offs = &g_kick_offs[self->rotation][0];
    } else {
        self->kick_offs = &g_kick_offs_cnt[self->rotation][0];
    }

    memcpy(
        self->tiles, g_pieces[self->id][self->rotation], sizeof(self->tiles)
    );
    update_tilemap(self);
}

void piece_rotate_90_cnt(struct Piece *self) {
    self->prev_rotation = self->rotation;

    if (self->rotation == 0) {
        self->rotation = 3;
    } else {
        --self->rotation;
    }

    if (self->id == PIECE_I) {
        self->kick_offs = &g_i_kick_offs[self->rotation][0];
    } else {
        self->kick_offs = &g_i_kick_offs_cnt[self->rotation][0];
    }

    memcpy(
        self->tiles, g_pieces[self->id][self->rotation], sizeof(self->tiles)
    );
    update_tilemap(self);
}

struct SDL_Point piece_get_kick_off(struct Piece *self, int idx) {
    if (self->id == PIECE_I) {
        return g_i_kick_offs[self->rotation][idx];
    } else {
        return g_kick_offs[self->rotation][idx];
    }
}

void piece_render(
    struct Piece *self, SDL_Renderer *renderer, struct TileSet *tileset,
    struct SDL_Point pos
) {
    for (int y = 0; y < PIECE_HEIGHT; ++y) {
        for (int x = 0; x < PIECE_WIDTH; ++x) {
            int tile_x = pos.x + x * TILE_WIDTH;
            int tile_y = pos.y + y * TILE_HEIGHT;

            tileset_render_tile(
                tileset, renderer, self->tilemap[y][x], tile_x, tile_y
            );
        }
    }
}
