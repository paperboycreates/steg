cmake_minimum_required(VERSION 3.9)
project(stag)

set(CMAKE_CXX_STANDARD 11)

add_executable(stag main.cpp lodepng.h lodepng.cpp steg.cpp steg.h)