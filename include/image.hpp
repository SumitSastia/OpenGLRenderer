#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <GLFW/glfw3.h>

struct image{

    GLFWimage* img;

    image(const char* path);
    ~image();
};

#endif