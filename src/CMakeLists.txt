# Copyright (c) 2024-present inunix3.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/wetris")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

set(HEADERS
    "${INCLUDE_DIR}/direction.h"
    "${INCLUDE_DIR}/font.h"
    "${INCLUDE_DIR}/font_store.h"
    "${INCLUDE_DIR}/game.h"
    "${INCLUDE_DIR}/input.h"
    "${INCLUDE_DIR}/piece.h"
    "${INCLUDE_DIR}/sfx_store.h"
    "${INCLUDE_DIR}/tetrion.h"
    "${INCLUDE_DIR}/text.h"
    "${INCLUDE_DIR}/tileset.h"
    "${INCLUDE_DIR}/timer.h"
    "${INCLUDE_DIR}/ui.h"
    "${INCLUDE_DIR}/utils.h"
)

set(SOURCES
    "${SRC_DIR}/font.c"
    "${SRC_DIR}/font_store.c"
    "${SRC_DIR}/game.c"
    "${SRC_DIR}/input.c"
    "${SRC_DIR}/main.c"
    "${SRC_DIR}/piece.c"
    "${SRC_DIR}/sfx_store.c"
    "${SRC_DIR}/tetrion.c"
    "${SRC_DIR}/text.c"
    "${SRC_DIR}/tileset.c"
    "${SRC_DIR}/timer.c"
    "${SRC_DIR}/ui.c"
    "${SRC_DIR}/utils.c"
)

add_executable(wetris ${HEADERS} ${SOURCES})

if (COMPILER STREQUAL "gcc" OR COMPILER STREQUAL "clang")
  set(COMPILE_OPTIONS
      -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -fstack-clash-protection
  )

  set(COMPILE_OPTIONS ${COMPILE_OPTIONS} $<$<CONFIG:Debug>:-Og>)
endif()

if (USE_ASAN)
  set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -fsanitize=address)
  target_link_options(wetris PRIVATE -fsanitize=address)
elseif (USE_UBSAN)
  set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -fsanitize=undefined)
  target_link_options(wetris PRIVATE -fsanitize=undefined)
endif()

if (WIN32)
    set_target_properties(wetris PROPERTIES WIN32_EXECUTABLE $<IF:$<CONFIG:Release>,ON,OFF>)
endif()

target_compile_options(wetris PRIVATE ${COMPILE_OPTIONS})
target_include_directories(wetris PRIVATE "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(
    wetris
    PRIVATE
    SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf
)

add_custom_command(
    TARGET wetris
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/assets" "${PROJECT_BINARY_DIR}/assets"
)

set(SDL_PATH $<TARGET_FILE:SDL3::SDL3>)
set(SDL_IMAGE_PATH $<TARGET_FILE:SDL3_image::SDL3_image>)
set(SDL_MIXER_PATH $<TARGET_FILE:SDL3_mixer::SDL3_mixer>)
set(SDL_TTF_PATH $<TARGET_FILE:SDL3_ttf::SDL3_ttf>)

if (LINUX)
    set(SDL_SONAME_PATH $<TARGET_SONAME_FILE:SDL3::SDL3>)
    set(SDL_IMAGE_SONAME_PATH $<TARGET_SONAME_FILE:SDL3_image::SDL3_image>)
    set(SDL_MIXER_SONAME_PATH $<TARGET_SONAME_FILE:SDL3_mixer::SDL3_mixer>)
    set(SDL_TTF_SONAME_PATH $<TARGET_SONAME_FILE:SDL3_ttf::SDL3_ttf>)

    file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/wetris.sh" CONTENT
"#!/bin/sh

WETRIS_PATH=\"${CMAKE_INSTALL_PREFIX}/share/games/wetris\"
export LD_LIBRARY_PATH=$WETRIS_PATH

cd $WETRIS_PATH
exec ./wetris
"
    )

    install(
        FILES "${PROJECT_BINARY_DIR}/wetris.sh"
        DESTINATION bin
        RENAME wetris
        # Corresponds to 775
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_WRITE GROUP_READ
        WORLD_EXECUTE WORLD_READ
    )

    install(TARGETS wetris DESTINATION share/games/wetris)
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION share/games/wetris)

    if (VENDORED_LIBS AND BUILD_SHARED_LIBS)
        install(
            FILES
            "${SDL_PATH}"
            "${SDL_IMAGE_PATH}"
            "${SDL_MIXER_PATH}"
            "${SDL_TTF_PATH}"
            "${SDL_SONAME_PATH}"
            "${SDL_IMAGE_SONAME_PATH}"
            "${SDL_MIXER_SONAME_PATH}"
            "${SDL_TTF_SONAME_PATH}"
            DESTINATION share/games/wetris
        )
    endif()
else()
    install(TARGETS wetris DESTINATION bin)
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets" DESTINATION bin)

    if (BUILD_SHARED_LIBS)
        install(
            FILES
            "${SDL_PATH}"
            "${SDL_IMAGE_PATH}"
            "${SDL_MIXER_PATH}"
            "${SDL_TTF_PATH}"
            DESTINATION bin
         )
    endif()
endif()
