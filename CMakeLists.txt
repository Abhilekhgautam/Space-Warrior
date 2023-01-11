cmake_minimum_required(VERSION 3.24)
project(two_dimensional_transformation)
set(CMAKE_CXX_STANDARD 17)
add_executable(two_dimensional_transformation main.cpp olcPixelGameEngine.h)
target_link_libraries(two_dimensional_transformation X11 GL pthread png "stdc++fs")

