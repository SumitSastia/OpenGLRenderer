#ifndef MYWINDOW_HPP
#define MYWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

constexpr auto WIN_W = 1120;
constexpr auto WIN_H = 700;

class shader {

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int shaderProgram;

public:

    shader(const char* vertPath, const char* fragPath);

    std::string loadShader(const char* path);
    const unsigned int& get_program() const { return shaderProgram; }
};

class line {

    unsigned int VBO;
    unsigned int VAO;

    float vertices[6];

public:

    ~line();

    void initLines(const glm::vec3 startPos, const glm::vec3 endPos);
    void updateLines(const glm::vec3 startPos, const glm::vec3 endPos);

    const unsigned int& get_VBO() const { return VBO; }
    const unsigned int& get_VAO() const { return VAO; }
};

class texture {

    int width;
    int height;
    int nrChannels;
    
    std::string type;
    unsigned int textureID;
    unsigned char* pixelData;
    
public:

    ~texture();

    void load(const char* path);

    const unsigned int& getID() const { return textureID; }
    const std::string& getType() const { return type; }
};

//class frame_buffer {
//
//    unsigned int FBO, RBO, VBO, VAO, EBO;
//    unsigned int frameTexture;
//
//public:
//
//    frame_buffer(const int& frameWidth, const int& frameHeight);
//    ~frame_buffer();
//
//    const unsigned int& get_FBO() const { return FBO; }
//    const unsigned int& get_VAO() const { return VAO; }
//    const unsigned int& get_TEX() const { return frameTexture; }
//};

struct material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct materials {

    material rubber, wood, plastic, concrete, glass, chrome;
    materials();

    static materials& instance();
};

void setInt(const unsigned int &shaderProgram, const char* target, const int &value);

void setFloat(const unsigned int &shaderProgram, const char* target, const float &value);

void setVec3(const unsigned int &shaderProgram, const char* target, const glm::vec3 &vector);

void setMat3(const unsigned int &shaderProgram, const char* target, const glm::mat3 &matrix);

void setMat4(const unsigned int &shaderProgram, const char* target, const glm::mat4 &matrix);

void setMaterial(const unsigned int& shaderProgram, const std::string &target);

void printVec3(const glm::vec3& vector);

#endif