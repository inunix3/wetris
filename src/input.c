/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/input.h>

struct InputState input_new(void) { return (struct InputState){0}; }

void input_update(struct InputState *self) {
    const bool *sdl_state = SDL_GetKeyboardState(NULL);

    Uint64 cur_time = SDL_GetTicks();

    for (SDL_Scancode key_id = 0; key_id < SDL_SCANCODE_COUNT; ++key_id) {
        struct InputKey *key = &self->keys[key_id];

        key->prev_state = key->state;

        if (sdl_state[key_id]) {
            if (key->state == INPUT_KEY_UP) {
                key->initial_press_time = cur_time;
            }

            key->state = INPUT_KEY_DOWN;
        } else {
            key->state = INPUT_KEY_UP;
        }
    }
}

bool input_key_down(struct InputState *self, SDL_Scancode key_id) {
    Uint64 cur_time = SDL_GetTicks();

    struct InputKey *key = &self->keys[key_id];

    if (key->state == INPUT_KEY_DOWN) {
        Uint64 since_initial_press = cur_time - key->initial_press_time;
        Uint64 since_last_press = cur_time - key->last_press_time;

        if (since_initial_press < REPEAT_DELAY) {
            if (key->prev_state == INPUT_KEY_UP) {
                return true;
            }
        } else if (since_last_press >= REPEAT_INTERVAL) {
            key->last_press_time = cur_time;

            return true;
        }
    }

    return false;
}
