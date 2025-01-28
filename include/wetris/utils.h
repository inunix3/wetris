/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include <SDL3/SDL.h>

void log_error(SDL_PRINTF_FORMAT_STRING const char *fmt, ...) SDL_PRINTF_VARARG_FUNC(1);
void log_sdl_error(void);
void *mem_alloc(size_t size);
void *mem_realloc(void *ptr, size_t size);
void mem_free(void *block);
