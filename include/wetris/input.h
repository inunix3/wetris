/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#pragma once

#include <SDL3/SDL.h>

#define REPEAT_DELAY 180
#define REPEAT_INTERVAL 75

enum InputKeyState { INPUT_KEY_UP, INPUT_KEY_DOWN };

struct InputKey {
    enum InputKeyState prev_state;
    enum InputKeyState state;

    /* Needed to simulate repeating */
    Uint64 last_press_time;
    Uint64 initial_press_time;
};

struct InputState {
    struct InputKey keys[SDL_SCANCODE_COUNT];
};

struct InputState input_new(void);
void input_update(struct InputState *self);

bool input_key_down(struct InputState *self, SDL_Scancode key_id);

static inline bool
input_key_released(const struct InputState *self, SDL_Scancode key) {
    return self->keys[key].state == INPUT_KEY_UP &&
           self->keys[key].prev_state == INPUT_KEY_DOWN;
}

static inline bool
input_key_pressed(const struct InputState *self, SDL_Scancode key) {
    return self->keys[key].state == INPUT_KEY_DOWN &&
           self->keys[key].prev_state == INPUT_KEY_UP;
}
