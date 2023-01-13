cmake_minimum_required(VERSION 3.24)
project(two_dimensional_transformation)
set(CMAKE_CXX_STANDARD 17)

include_directories(music)
include_directories(sprites)

add_executable(two_dimensional_transformation main.cpp olcPixelGameEngine.h olcSoundWaveEngine.h)
target_link_libraries(two_dimensional_transformation pulse pulse-simple X11 GL pthread png  "stdc++fs")
