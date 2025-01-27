/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Timer {
    uint64_t start_time;
    uint64_t interval;
};

struct Timer timer_new(uint64_t interval);
void timer_restart(struct Timer *self);
uint64_t timer_elapsed(const struct Timer *self);
bool timer_done(const struct Timer *self);
