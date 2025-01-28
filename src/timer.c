/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/timer.h>

#include <SDL3/SDL.h>

struct Timer timer_new(uint64_t interval) {
    struct Timer timer = {
        .start_time = SDL_GetTicks(),
        .interval = interval,
    };

    return timer;
}

void timer_restart(struct Timer *self) { self->start_time = SDL_GetTicks(); }

uint64_t timer_elapsed(const struct Timer *self) {
    return SDL_GetTicks() - self->start_time;
}

bool timer_done(const struct Timer *self) {
    return timer_elapsed(self) >= self->interval;
}
