#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class engine{

    static engine* pointer;

public:

    engine();
    ~engine();

    engine* get_instance(){
        return pointer = (pointer != nullptr)? pointer : new engine();
    }
};

#endif