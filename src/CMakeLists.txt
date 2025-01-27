# Copyright (c) 2024-present inunix3.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

set(BIN_NAME ${PROJECT_NAME})
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}")
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

add_executable(${BIN_NAME} ${HEADERS} ${SOURCES})

if (COMPILER STREQUAL "gcc" OR COMPILER STREQUAL "clang")
    set(COMPILE_OPTIONS -Wall -Wextra -Wconversion -Wsign-conversion -Wshadow -fstack-clash-protection)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -Og)
    endif()
endif()

if (USE_ASAN)
    set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -fsanitize=address)
    target_link_options(${BIN_NAME} PRIVATE -fsanitize=address)
elseif(USE_UBSAN)
    set(COMPILE_OPTIONS ${COMPILE_OPTIONS} -fsanitize=undefined)
    target_link_options(${BIN_NAME} PRIVATE -fsanitize=undefined)
endif()

set_target_properties(${BIN_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>)
target_compile_options(${BIN_NAME} PRIVATE ${COMPILE_OPTIONS})
target_include_directories(${BIN_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${BIN_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf)

install(TARGETS ${BIN_NAME} DESTINATION bin)
