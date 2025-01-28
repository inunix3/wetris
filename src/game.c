/*
 * Copyright (c) 2024-present inunix3
 *
 * This file is licensed under the MIT License (see LICENSE.md).
 */

#include <wetris/game.h>
#include <wetris/utils.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void handle_input(struct Game *self) {
    if (self->state == GAME_RUNNING) {
        tetrion_handle_input(&self->tetrion, &self->input);
    }

    if (input_key_released(&self->input, SDL_SCANCODE_P) &&
        (self->state == GAME_RUNNING || self->state == GAME_PAUSED) &&
        (self->tetrion.state == TETRION_STATE_NORMAL ||
         self->tetrion.state == TETRION_STATE_UPDATING_ROWS)) {
        if (self->state == GAME_RUNNING) {
            self->state = GAME_PAUSED;

            ui_show_text(&self->ui, TEXT_PAUSED);
            tetrion_handle_pause(&self->tetrion);
        } else {
            self->state = GAME_RUNNING;

            ui_hide_text(&self->ui, TEXT_PAUSED);
        }

        sfx_store_play(&self->sfx_store, SFX_LEVEL_UP);
    }

    if (input_key_released(&self->input, SDL_SCANCODE_ESCAPE)) {
        self->state = GAME_QUIT;
    }
}

static void handle_event(struct Game *self, const SDL_Event *event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        self->state = GAME_QUIT;

        break;
    default:
        break;
    }
}

static void update(struct Game *self) { tetrion_update(&self->tetrion); }

static void render(struct Game *self) {
    SDL_SetRenderDrawColor(self->renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(self->renderer);

    SDL_RenderTextureTiled(self->renderer, self->background, NULL, 1, NULL);
    tetrion_render(&self->tetrion, self->renderer, &self->tileset);

    ui_render(&self->ui);

    SDL_RenderPresent(self->renderer);
}

static bool init_sdl(void) {
    SDL_SetAppMetadata("wetris", "1.0", "com.inunix3.wetris");
    SDL_srand(0);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        log_sdl_error();

        return false;
    }

    if (!TTF_Init()) {
        log_sdl_error();
        SDL_Quit();

        return false;
    }

    struct SDL_AudioSpec spec = {0};

    spec.freq = MIX_DEFAULT_FREQUENCY;
    spec.format = MIX_DEFAULT_FORMAT;
    spec.channels = MIX_DEFAULT_CHANNELS;

    if (!Mix_OpenAudio(0, &spec)) {
        log_sdl_error();
        TTF_Quit();
        SDL_Quit();

        return false;
    }

    Mix_AllocateChannels(TOTAL_SFX);

    return true;
}

static void set_icon(struct Game *self, const char *path) {
    SDL_Surface *icon = IMG_Load(path);

    if (!icon) {
        log_sdl_error();
    }

    SDL_SetWindowIcon(self->window, icon);
    SDL_DestroySurface(icon);
}

static bool init_window(struct Game *self) {
    self->window =
    SDL_CreateWindow("Wetris", self->width, self->height, 0);

    if (!self->window) {
        log_sdl_error();

        return false;
    }

    set_icon(self, "./assets/icon.png");

    self->renderer = SDL_CreateRenderer(self->window, NULL);

    if (!self->renderer) {
        log_sdl_error();

        return false;
    }

    return true;
}

static bool load_assets(struct Game *self) {
    self->background =
    IMG_LoadTexture(self->renderer, "./assets/background.png");

    if (!self->background) {
        log_sdl_error();

        return false;
    }

    if (!tileset_load(&self->tileset, self->renderer, "./assets/tileset.png")) {
        return false;
    }

    if (!font_store_init(&self->font_store, self->renderer)) {
        return false;
    }

    if (!sfx_store_init(&self->sfx_store)) {
        return false;
    }

    return true;
}

struct Game *game_alloc(void) {
    return mem_alloc(sizeof(struct Game));
}

void game_free(struct Game *self) {
    game_deinit(self);
    mem_free(self);
}

bool game_init(struct Game *self) {
    self->width = WINDOW_WIDTH;
    self->height = WINDOW_HEIGHT;

    if (!init_sdl()) {
        return false;
    }

    if (!init_window(self)) {
        goto failure;
    }

    if (!load_assets(self)) {
        goto failure;
    }

    int tetrion_x = TETRION_PADDING_LEFT;
    int tetrion_y = 0;

    if (!tetrion_init(
        &self->tetrion, &self->sfx_store, &self->ui, tetrion_x, tetrion_y,
        TETRION_WIDTH, TETRION_HEIGHT
    )) {
        game_deinit(self);

        return NULL;
    }

    self->input = input_new();

    ui_init(&self->ui, self);
    ui_show_text(&self->ui, TEXT_PRESS_SPACE);

    self->state = GAME_RUNNING;

    return true;

failure:
    log_sdl_error();
    game_deinit(self);

    return false;
}

void game_deinit(struct Game *self) {
    ui_deinit(&self->ui);
    tetrion_deinit(&self->tetrion);
    sfx_store_deinit(&self->sfx_store);
    font_store_deinit(&self->font_store);
    tileset_deinit(&self->tileset);

    SDL_DestroyTexture(self->background);
    self->background = NULL;

    SDL_DestroyRenderer(self->renderer);
    self->renderer = NULL;

    SDL_DestroyWindow(self->window);
    self->window = NULL;

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void game_run(struct Game *self) {
    Uint64 dt = 1000 / DEFAULT_FPS;

    while (self->state != GAME_QUIT) {
        Uint64 start_time = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            handle_event(self, &event);
        }

        input_update(&self->input);
        handle_input(self);

        if (self->state == GAME_RUNNING) {
            update(self);
        }

        render(self);

        SDL_DelayPrecise((start_time + dt - SDL_GetTicks()) * 1000000);
    }
}
