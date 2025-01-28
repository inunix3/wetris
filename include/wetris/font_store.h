/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include "font.h"

#include <stdbool.h>

enum FontId {
    FONT_SMALL,
    FONT_BIG,
    FONT_LARGE,

    TOTAL_FONTS
};

struct FontStore {
    struct Font fonts[TOTAL_FONTS];
};

bool font_store_init(struct FontStore *self, struct SDL_Renderer *renderer);
void font_store_deinit(struct FontStore *self);
