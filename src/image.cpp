#include <image.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

image::image(const char* path){

    img = new GLFWimage();
    img->pixels = stbi_load(path,&img->width,&img->height,nullptr,4);
}

image::~image(){
    stbi_image_free(img->pixels);
}