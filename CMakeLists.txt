cmake_minimum_required(VERSION 3.29)
project(snake)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 CONFIG REQUIRED)

add_executable(snake main.cpp)

target_link_libraries(snake PRIVATE SDL2::SDL2 SDL2::SDL2main)
