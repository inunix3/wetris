/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/sfx_store.h>

#include <wetris/utils.h>

bool sfx_store_init(struct SfxStore *self) {
    const char *names[TOTAL_SFX] = {
        /* clang-format off */
        "./assets/landed.wav",
        "./assets/drop.wav",
        "./assets/move.wav",
        "./assets/rotate.wav",
        "./assets/level_up.wav",
        "./assets/deleted_row.wav",
        "./assets/game_over.wav",
        /* clang-format on */
    };

    for (size_t i = 0; i < TOTAL_SFX; ++i) {
        self->sfx[i] = Mix_LoadWAV(names[i]);

        if (!self->sfx[i]) {
            log_sdl_error();

            return false;
        }
    }

    return true;
}

void sfx_store_deinit(struct SfxStore *self) {
    if (!self) {
        return;
    }

    for (size_t i = 0; i < TOTAL_SFX; ++i) {
        Mix_FreeChunk(self->sfx[i]);
        self->sfx[i] = NULL;
    }
}

void sfx_store_play(struct SfxStore *self, enum SfxId id) {
    /* Each sfx has its own channel */
    int channel = (int)id;

    Mix_PlayChannel(channel, self->sfx[id], 0);
}
