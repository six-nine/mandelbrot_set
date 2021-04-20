cmake_minimum_required(VERSION 3.17)
project(Mandelbrot)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Mandelbrot main.cpp)
target_link_libraries(Mandelbrot ${SDL2_LIBRARIES})

