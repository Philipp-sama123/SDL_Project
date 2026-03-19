cmake_minimum_required(VERSION 3.20)
project(SDL3App LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/SDL3)

add_executable(SDL3App src/main.cpp)

target_link_libraries(SDL3App PRIVATE SDL3::SDL3)