cmake_minimum_required(VERSION 3.20)
project(SDL_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Disable optional features BEFORE adding SDL_image ----
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)

# ---- SDL ----
add_subdirectory(external/SDL3)

# ---- SDL_image ----
add_subdirectory(external/SDL_image)

# ---- Game ----
add_executable(SDL_Project
    src/main.cpp
    src/Animator.cpp
)

# ---- Link dependencies ----
target_link_libraries(SDL_Project PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

# ---- Includes ----
target_include_directories(SDL_Project PRIVATE src)

# ---- Compiler warnings ----
if(MSVC)
    target_compile_options(SDL_Project PRIVATE /W4 /permissive-)
else()
    target_compile_options(SDL_Project PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- Copy SDL DLL (Windows) ----
if(WIN32)
    add_custom_command(TARGET SDL_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:SDL_Project>
    )
endif()
# ---- Copy IMG SDL DLL (Windows) ----
if(WIN32 AND TARGET SDL3_image::SDL3_image)
    add_custom_command(TARGET SDL_Project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:SDL_Project>
    )
endif()
# ---- Copy Images to 
add_custom_command(TARGET SDL_Project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:SDL_Project>/assets
)

set_target_properties(SDL_Project PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SDL_Project>"
)