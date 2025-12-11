#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class engine{

    static engine* pointer;

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

    unsigned int VBO;
    unsigned int VAO;

public:

    engine();

    void init(const float* vertices, const size_t size);
    void destroy();

    unsigned int get_VAO() const { return VAO; }
    unsigned int get_program() const { return shaderProgram; }

    static engine* get_instance(){
        return pointer = (pointer != nullptr)? pointer : new engine();
    }
};

#endif