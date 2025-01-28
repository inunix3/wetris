# Copyright (c) 2024-present inunix3.
# This file is distributed under the MIT license (https://opensource.org/license/mit/)

cmake_minimum_required(VERSION 3.11)

project(wetris VERSION 1.0.0 LANGUAGES C)

option(VENDORED_LIBS "Use vendored libs" OFF)
option(USE_ASAN "Enable AddressSanitizer" OFF)
option(USE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILER "gcc")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    set(COMPILER "clang")
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    set(COMPILER "clang-cl")
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER "msvc")
else()
    message(WARNING "compiler is unknown to ${PROJECT_NAME}")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(third-party)
add_subdirectory(src)
