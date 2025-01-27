# Copyright (c) 2024-present inunix3.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

if (VENDORED_LIBS)
    add_subdirectory(SDL)
    add_subdirectory(SDL_image)
    add_subdirectory(SDL_mixer)
    add_subdirectory(SDL_ttf)
else()
    find_package(SDL3 REQUIRED CONFIG)
    find_package(SDL3_image REQUIRED CONFIG)
    find_package(SDL3_mixer REQUIRED CONFIG)
    find_package(SDL3_ttf REQUIRED CONFIG)
endif()
