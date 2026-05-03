cmake_minimum_required(VERSION 3.20)

project(SDL_Project
        VERSION 1.0
        LANGUAGES CXX
)

# -------------------------
# Global settings
# -------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SDL_SHARED "Build SDL as shared" ON)

# -------------------------
# Dependencies (modern approach)
# -------------------------
include(FetchContent)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG preview-3.1.8
)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG preview-3.1.0
)

set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3 SDL3_image)

# -------------------------
# Game target
# -------------------------
add_executable(SDL_Project
        src/main.cpp
        src/Animator.cpp
)

target_include_directories(SDL_Project PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(SDL_Project PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
)

# -------------------------
# Assets handling (cross-platform safe)
# -------------------------
add_custom_command(TARGET SDL_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:SDL_Project>/assets
)

# -------------------------
# Warnings (cross-platform)
# -------------------------
if (MSVC)
    target_compile_options(SDL_Project PRIVATE /W4 /permissive-)
else()
    target_compile_options(SDL_Project PRIVATE -Wall -Wextra -Wpedantic)
endif()

# -------------------------
# macOS bundle support (optional)
# -------------------------
if (APPLE)
    set_target_properties(SDL_Project PROPERTIES
            MACOSX_BUNDLE TRUE
    )
endif()

# -------------------------
# Windows runtime DLL copy (only when needed)
# -------------------------
if (WIN32)
    add_custom_command(TARGET SDL_Project POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:SDL_Project>
    )

    add_custom_command(TARGET SDL_Project POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:SDL_Project>
    )
endif()