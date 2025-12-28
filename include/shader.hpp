#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class shader{

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

public:

    shader(const char* vertPath, const char* fragPath);
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
    void init2(const float* vertices, size_t size_v, const unsigned int* indices, size_t size_i);

    unsigned int get_VBO() const { return VBO; }
    unsigned int get_VAO() const { return VAO; }
    unsigned int get_EBO() const { return EBO; }
};

class line{

    unsigned int VBO;
    unsigned int VAO;

    float vertices[6];

public:

    ~line();

    void initLines(const glm::vec3 startPos, const glm::vec3 endPos);
    void updateLines(const glm::vec3 startPos, const glm::vec3 endPos);

    unsigned int get_VBO() const { return VBO; }
    unsigned int get_VAO() const { return VAO; }
};

class texture{

    int width;
    int height;
    int nrChannels;

    unsigned int textureID;
    unsigned char* pixelData;

public:

    texture();
    ~texture();

    void load(const char* path);
    unsigned int getID() const { return textureID; }
};

struct material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct materials{

    material rubber, wood, plastic, concrete, glass, chrome;
    materials();
};

void setFloat(const unsigned int &shaderProgram, const char* target, const float &value);

void setVec3(const unsigned int &shaderProgram, const char* target, const glm::vec3 &vector);

void setMat3(const unsigned int &shaderProgram, const char* target, const glm::mat3 &matrix);

void setMat4(const unsigned int &shaderProgram, const char* target, const glm::mat4 &matrix);

#endif