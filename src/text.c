/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/text.h>

#include <wetris/utils.h>

#define TEMP_BUF_SIZE (511 + 1)

static char g_temp_buf[TEMP_BUF_SIZE];

static void render_left_aligned(const struct Text *self, int x, int y) {
    font_render_str(self->font, self->data, x, y, self->fg);
}

static void render_center_aligned(const struct Text *self, int x, int y) {
    const int initial_x = x;

    for (size_t i = 0; i < self->line_count; ++i) {
        const struct Line *line = &self->lines[i];

        x += self->rect.w / 2 - line->w / 2;

        font_render_str_n(self->font, line->begin, line->len, x, y, self->fg);

        y += line->h;
        x = initial_x;
    }
}

static void render_right_aligned(const struct Text *self, int x, int y) {
    const int initial_x = x;

    for (size_t i = 0; i < self->line_count; ++i) {
        const struct Line *line = &self->lines[i];

        x += self->rect.w - line->w;

        font_render_str_n(self->font, line->begin, line->len, x, y, self->fg);

        y += line->h;
        x = initial_x;
    }
}

static bool map_lines(struct Text *self) {
    /* TODO: this code would be better if we introduce a vector (dynamic array). */

    struct Line cur_line = {self->data, 0, 0, 0};

    for (char *ch = self->data;; ++ch) {
        if (*ch != '\0' && *ch != '\n') {
            ++cur_line.len;

            continue;
        }

        if (self->line_count >= self->line_capacity) {
            size_t new_capacity = self->line_capacity + 5;

            self->lines =
            mem_realloc(self->lines, new_capacity * sizeof(struct Line));

            if (!self->lines) {
                return false;
            }

            self->line_capacity = new_capacity;
        }

        if (*ch == '\n' && cur_line.len == 0) {
            ++cur_line.len;
        } else if (*ch == '\0' && cur_line.len == 0) {
            break;
        }

        self->lines[self->line_count++] = cur_line;

        cur_line.begin = ch + 1;
        cur_line.len = 0;

        if (*ch == '\0') {
            break;
        }
    }

    for (size_t i = 0; i < self->line_count; ++i) {
        struct Line *line = &self->lines[i];

        font_measure_n(self->font, line->begin, line->len, &line->w, &line->h);
    }

    return true;
}

void text_init(
    struct Text *self, struct Font *font, enum TextAlignment align,
    struct SDL_Color fg
) {
    self->font = font;
    self->align = align;
    self->data = NULL;
    self->lines = NULL;
    self->fg = fg;
    self->show = true;
}

void text_deinit(struct Text *self) {
    if (!self) {
        return;
    }

    mem_free(self->lines);
    self->lines = NULL;
    self->line_count = 0;
    self->line_capacity = 0;

    mem_free(self->data);
    self->data = NULL;
}

void text_render(struct Text *self) {
    if (!self->show) {
        return;
    }

    int x = self->rect.x;
    int y = self->rect.y;

    switch (self->align) {
    case TEXT_ALIGN_LEFT:
        render_left_aligned(self, x, y);

        break;
    case TEXT_ALIGN_CENTER:
        render_center_aligned(self, x, y);

        break;
    case TEXT_ALIGN_RIGHT:
        render_right_aligned(self, x, y);

        break;
    }
}

bool text_set(struct Text *self, const char *fmt, ...) {
    va_list vargs;

    va_start(vargs, fmt);
    bool result = text_vset(self, fmt, vargs);
    va_end(vargs);

    return result;
}

bool text_vset(struct Text *self, const char *fmt, va_list vargs) {
    SDL_vsnprintf(g_temp_buf, TEMP_BUF_SIZE, fmt, vargs);

    size_t len = strlen(g_temp_buf) + 1;
    self->data = mem_realloc(self->data, len);

    if (!self->data) {
        return false;
    }

    strncpy(self->data, g_temp_buf, len);

    self->len = len - 1;
    font_measure(self->font, fmt, &self->rect.w, &self->rect.h);

    /* TODO: this code would be much better if we'd introduce a vector
     * abstraction (dynamic array). */

    self->line_count = 0;
    self->line_capacity = 5;
    self->lines =
        mem_realloc(self->lines, self->line_capacity * sizeof(struct Line));

    if (!self->lines) {
        text_deinit(self);

        return false;
    }

    return map_lines(self);
}
