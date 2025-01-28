/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdbool.h>

#define TOTAL_GLYPHS 128 /* covers whole ASCII table */
#define FONT_MAX_TEXTURE_SIZE 512

struct Font {
    struct TTF_Font *ttf;
    struct SDL_Rect glyph_rects[TOTAL_GLYPHS];
    struct SDL_Texture *glyph_atlas;
    struct SDL_Renderer *renderer;

    int line_spacing;
    int height;
};

bool font_open_ttf(
    struct Font *self, SDL_Renderer *renderer, const char *path,
    float point_size
);
void font_close(struct Font *self);
void font_render_char(
    const struct Font *self, char ch, int x, int y, struct SDL_Color fg
);
void font_render_str(
    const struct Font *self, const char *text, int x, int y, struct SDL_Color fg
);
void font_render_str_n(
    const struct Font *self, const char *text, size_t len, int x, int y,
    struct SDL_Color fg
);
void font_measure(const struct Font *self, const char *text, int *w, int *h);
void font_measure_n(
    const struct Font *self, const char *text, size_t len, int *w, int *h
);
