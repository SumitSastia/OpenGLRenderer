cmake_minimum_required(VERSION 4.0)
project(GLFWproject)

# Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Executable
set(TARGET_NAME firstGame)

add_executable(${TARGET_NAME}
    src/main.cpp
    src/glad.c
    src/shader.cpp
)

# including headers & linkers
target_include_directories(${TARGET_NAME} PRIVATE include)
target_link_directories(${TARGET_NAME} PRIVATE libs)

# libraries
# add_subdirectory(libs)
target_link_libraries(${TARGET_NAME} glfw3 opengl32)