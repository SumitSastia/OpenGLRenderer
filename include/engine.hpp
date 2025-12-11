#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class engine{

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

public:

    engine();
    void destroy();

    std::string loadShader(const char* path);
    unsigned int get_program() const { return shaderProgram; }
};

class buffer{

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

public:

    void destroy();
    void init(const float* vertices, size_t size_v, const unsigned int* indices, size_t size_i);

    unsigned int get_VAO() const { return VAO; }
    unsigned int get_EBO() const { return EBO; }
};

#endif