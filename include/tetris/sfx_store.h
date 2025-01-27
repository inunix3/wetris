/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include <stdbool.h>

enum SfxId {
    SFX_LANDED,
    SFX_DROP,
    SFX_MOVE,
    SFX_ROTATE,
    SFX_LEVEL_UP,
    SFX_DELETED_ROW,
    SFX_GAME_OVER,

    TOTAL_SFX
};

struct SfxStore {
    struct Mix_Chunk *sfx[TOTAL_SFX];
};

bool sfx_store_init(struct SfxStore *self);
void sfx_store_deinit(struct SfxStore *self);
void sfx_store_play(struct SfxStore *self, enum SfxId id);
