/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/utils.h>

#include <SDL3/SDL.h>

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void log_error(const char *fmt, ...) {
    va_list vargs;

    va_start(vargs, fmt);
    SDL_LogMessageV(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, vargs
    );
    va_end(vargs);
}

void log_sdl_error(void) { log_error("%s", SDL_GetError()); }

void *mem_alloc(size_t size) {
    void *block = SDL_malloc(size);

    if (!block) {
        log_error("cannot allocate %zu bytes: out of memory", size);
    }

    return block;
}

void *mem_realloc(void *block, size_t size) {
    void *new_block = SDL_realloc(block, size);

    if (!new_block) {
        log_error("cannot reallocate %zu bytes: out of memory", size);
    }

    return new_block;
}

void mem_free(void *block) { SDL_free(block); }
