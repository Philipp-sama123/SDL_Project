cmake_minimum_required(VERSION 3.20)
project(SDL3App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If you use vcpkg, pass the toolchain file:
# (invoke cmake with -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg.cmake)

find_package(SDL3 REQUIRED CONFIG)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Copy DLL for Windows builds
if (WIN32 AND TARGET SDL3::SDL3-shared)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy
            $<TARGET_FILE:SDL3::SDL3-shared>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()