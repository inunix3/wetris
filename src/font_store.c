/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <tetris/font_store.h>

bool font_store_init(struct FontStore *self, struct SDL_Renderer *renderer) {
    const char *filenames[TOTAL_FONTS] = {
        "./assets/small.ttf", "./assets/big.ttf", "./assets/large.ttf"
    };

    float sizes[TOTAL_FONTS] = {8.f, 16.f, 24.f};

    for (size_t i = 0; i < TOTAL_FONTS; ++i) {
        if (!font_open_ttf(&self->fonts[i], renderer, filenames[i], sizes[i])) {

            return false;
        }
    }

    self->fonts[FONT_SMALL].line_spacing = 2;

    return true;
}

void font_store_deinit(struct FontStore *self) {
    if (!self) {
        return;
    }

    for (size_t i = 0; i < TOTAL_FONTS; ++i) {
        font_close(&self->fonts[i]);
    }
}
