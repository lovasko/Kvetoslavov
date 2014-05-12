cmake_minimum_required(VERSION 2.8)
project(Kvetoslavov)

file(GLOB_RECURSE Svit_SOURCES "src/*.c")
file(GLOB_RECURSE Svit_HEADERS "src/*.h")

include_directories("src/")

list(APPEND CMAKE_C_FLAGS " -Wall -Wextra ${CMAKE_C_FLAGS} ")

add_executable (Kvetoslavov ${Svit_SOURCES})

