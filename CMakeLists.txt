cmake_minimum_required(VERSION 4.0)
project(firstproject)

# Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Executable
set(TARGET_NAME firstGame)
add_executable(${TARGET_NAME} main.cpp)

# including headers
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(libs)
target_link_libraries(${TARGET_NAME} lib1)