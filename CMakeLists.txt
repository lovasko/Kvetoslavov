cmake_minimum_required(VERSION 2.8)
project(Kvetoslavov)

file(GLOB_RECURSE Kvetoslavov_SOURCES "src/*.c")
file(GLOB_RECURSE Kvetoslavov_HEADERS "src/*.h")

include_directories("src/")

list(APPEND CMAKE_C_FLAGS " -Wall -Wextra ${CMAKE_C_FLAGS} ")

add_executable (kvet ${Kvetoslavov_SOURCES})
target_link_libraries(kvet kvm)
target_link_libraries(kvet procstat)

