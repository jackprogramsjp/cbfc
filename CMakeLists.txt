cmake_minimum_required(VERSION 3.17)
project(cbfc C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -Wall -Wextra -Wpedantic")

set(CMAKE_C_STANDARD 90)

add_executable(cbfc main.c cbfc.c cbfc.h)