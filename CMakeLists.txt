cmake_minimum_required(VERSION 4.0)
project(firstproject)

add_executable(firstgame main.cpp)
add_library(lib1 SHARED f1.cpp)

target_link_libraries(firstgame PRIVATE lib1)