/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <tetris/font.h>

#include <tetris/utils.h>

static bool is_ascii(int ch) {
    /* Includes all digits, small and capitalized latin letters, punctuation and
     * newline. */
    return ch == '\n' || (ch >= ' ' && ch <= '~');
}

static bool add_glyph(
    struct Font *self, struct SDL_Surface *atlas, Uint32 ch,
    struct SDL_Rect *glyph_rect
) {
    const SDL_Color COLOR_WHITE = {0xff, 0xff, 0xff, 0xff};

    SDL_Surface *glyph_surface =
        TTF_RenderGlyph_Blended(self->ttf, ch, COLOR_WHITE);

    if (!glyph_surface) {
        log_sdl_error();

        return false;
    }

    char ch_str[2] = {(char)ch, '\0'};

    TTF_GetStringSize(self->ttf, ch_str, 1, &glyph_rect->w, &glyph_rect->h);

    if (ch == '\n') {
        glyph_rect->w = 0;
    } else if (ch == '\0') {
        glyph_rect->w = 0;
        glyph_rect->h = 0;
    }

    if (glyph_rect->x + glyph_rect->w >= FONT_MAX_TEXTURE_SIZE) {
        glyph_rect->x = 0;
        glyph_rect->y += glyph_rect->h;

        if (glyph_rect->y + glyph_rect->h >= FONT_MAX_TEXTURE_SIZE) {
            log_error("out of glyph space: max size of glyph atlas texture exceeded"
            );
            font_close(self);

            return false;
        }
    }

    SDL_BlitSurface(glyph_surface, NULL, atlas, glyph_rect);

    self->glyph_rects[ch] = *glyph_rect;

    glyph_rect->x += glyph_rect->w;

    SDL_DestroySurface(glyph_surface);

    return true;
}

bool font_open_ttf(
    struct Font *self, struct SDL_Renderer *renderer, const char *path,
    float point_size
) {
    self->ttf = TTF_OpenFont(path, point_size);

    if (!self->ttf) {
        log_sdl_error();

        return false;
    }

    self->height = TTF_GetFontHeight(self->ttf);

    SDL_Surface *atlas = SDL_CreateSurface(
        FONT_MAX_TEXTURE_SIZE, FONT_MAX_TEXTURE_SIZE, SDL_PIXELFORMAT_RGBA32
    );

    if (!atlas) {
        log_sdl_error();
        font_close(self);

        return false;
    }

    // Black color will be used as a transparent color.
    // The glyphs itself will be white.
    SDL_SetSurfaceColorKey(atlas, true, SDL_MapSurfaceRGBA(atlas, 0, 0, 0, 0));

    /* Holds size of glyph and its position in the glyph atlas. */
    SDL_Rect glyph_rect = {0};

    for (Uint32 ch = ' '; ch <= '~'; ++ch) {
        if (!add_glyph(self, atlas, ch, &glyph_rect)) {
            goto failure;
        }
    }

    if (!add_glyph(self, atlas, '\n', &glyph_rect)) {
        goto failure;
    }

    if (!add_glyph(self, atlas, '\0', &glyph_rect)) {
        goto failure;
    }

    self->glyph_atlas = SDL_CreateTextureFromSurface(renderer, atlas);

    if (!self->glyph_atlas) {
        log_sdl_error();

        goto failure;
    }

    SDL_DestroySurface(atlas);

    self->renderer = renderer;
    self->line_spacing = 0;

    return true;

failure:
    SDL_DestroySurface(atlas);
    font_close(self);

    return false;
}

void font_close(struct Font *self) {
    if (!self) {
        return;
    }

    SDL_DestroyTexture(self->glyph_atlas);
    self->glyph_atlas = NULL;

    TTF_CloseFont(self->ttf);
    self->ttf = NULL;
}

void font_render_char(
    const struct Font *self, char ch, int x, int y, struct SDL_Color fg
) {
    if (!is_ascii(ch)) {
        return;
    }

    const SDL_Rect *glyph_rect = &self->glyph_rects[(size_t)ch];
    SDL_Rect screen_rect = {
        .x = x,
        .y = y,
        .w = glyph_rect->w,
        .h = glyph_rect->h,
    };

    SDL_FRect glyph_frect;
    SDL_FRect screen_frect;

    SDL_RectToFRect(&self->glyph_rects[(size_t)ch], &glyph_frect);
    SDL_RectToFRect(&screen_rect, &screen_frect);

    SDL_SetTextureColorMod(self->glyph_atlas, fg.r, fg.g, fg.b);
    SDL_RenderTexture(
        self->renderer, self->glyph_atlas, &glyph_frect, &screen_frect
    );
}

void font_render_str(
    const struct Font *self, const char *text, int x, int y, struct SDL_Color fg
) {
    font_render_str_n(self, text, strlen(text), x, y, fg);
}

void font_render_str_n(
    const struct Font *self, const char *text, size_t len, int x, int y,
    struct SDL_Color fg
) {
    int initial_x = x;

    for (size_t i = 0; i < len; ++i) {
        char ch = text[i];

        if (ch == '\n') {
            x = initial_x;
            y += TTF_GetFontHeight(self->ttf) + self->line_spacing;
        } else {
            font_render_char(self, ch, x, y, fg);
        }

        x += self->glyph_rects[(size_t)ch].w;
    }
}

void font_measure(const struct Font *self, const char *text, int *w, int *h) {
    return font_measure_n(self, text, strlen(text), w, h);
}

void font_measure_n(
    const struct Font *self, const char *text, size_t len, int *w, int *h
) {
    int max_w = 0;
    int max_h = 0;
    int cur_w = 0;
    int cur_h = 0;

    for (size_t i = 0; i < len + 1; ++i) {
        char ch = text[i];
        int ch_w = self->glyph_rects[(size_t)ch].w;
        int ch_h = self->glyph_rects[(size_t)ch].h;

        cur_w += ch_w;
        cur_h = SDL_max(cur_h, ch_h);

        switch (ch) {
        case '\0':
        case '\n':
            if (ch == '\n') {
                cur_h += self->line_spacing;
            }

            max_w = SDL_max(max_w, cur_w);
            max_h += cur_h;
            cur_w = 0;
            cur_h = 0;

            break;
        default:
            break;
        }
    }

    if (w) {
        *w = max_w;
    }

    if (h) {
        *h = max_h;
    }
}
