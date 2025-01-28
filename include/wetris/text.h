/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include "font.h"

#include <SDL3/SDL.h>

#include <stdarg.h>

enum TextAlignment { TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT };

struct Line {
    char *begin;
    size_t len;
    int w, h;
};

struct Text {
    struct Font *font;
    enum TextAlignment align;
    char *data;
    size_t len;
    struct Line *lines;
    size_t line_count;
    size_t line_capacity;
    struct SDL_Rect rect;
    struct SDL_Color fg;
    bool show;
};

void text_init(
    struct Text *self, struct Font *font, enum TextAlignment align,
    struct SDL_Color fg
);

void text_deinit(struct Text *self);

bool text_set(struct Text *self, SDL_PRINTF_FORMAT_STRING const char *data, ...)
    SDL_PRINTF_VARARG_FUNC(2);

bool text_vset(
    struct Text *self, SDL_PRINTF_FORMAT_STRING const char *data, va_list vargs
) SDL_PRINTF_VARARG_FUNCV(2);

void text_render(struct Text *self);
