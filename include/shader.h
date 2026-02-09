#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

constexpr auto WIN_W = 1120;
constexpr auto WIN_H = 700;

std::string loadShaderFile(const char* path);
const unsigned int& createShader(const char* vertPath, const char* fragPath);
const unsigned int& createShader2(const char* vertPath, const char* geomPath, const char* fragPath);

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

class frame_buffer {

    unsigned int FBO, RBO, VBO, VAO, EBO;
    unsigned int frameTexture;

public:

    frame_buffer(const int& frameWidth, const int& frameHeight);
    ~frame_buffer();

    const unsigned int& get_FBO() const { return FBO; }
    const unsigned int& get_VAO() const { return VAO; }
    const unsigned int& get_TEX() const { return frameTexture; }
};

class shadowFrameBuffer {

    unsigned int FBO, RBO, VBO, VAO, EBO;
    unsigned int depthMap;

public:

    shadowFrameBuffer(const int& shadowWidth, const int& shadowHeight);
    ~shadowFrameBuffer();

    const unsigned int& get_FBO() const { return FBO; }
    const unsigned int& get_VAO() const { return VAO; }
    const unsigned int& get_depthMap() const { return depthMap; }
};

struct material {

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    material() : shininess(0) {}

    material(
        const glm::vec3 ambient,
        const glm::vec3 diffuse,
        const glm::vec3 specular,
        const float shininess
    ) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        shininess(shininess) {
    }
};

namespace materials {

    const material rubber   { glm::vec3(0.02f), glm::vec3(0.01f), glm::vec3(0.4f), 10.0f };
    const material wood     { glm::vec3(0.2f, 0.15f, 0.1f), glm::vec3(0.4f, 0.3f, 0.2f), glm::vec3(0.05f), 8.0f };
    const material plastic  { glm::vec3(0.1f), glm::vec3(0.6f), glm::vec3(0.2f), 16.0f };
    const material concrete { glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(0.1f), 4.0f };
    const material glass    { glm::vec3(0.0f), glm::vec3(0.3f), glm::vec3(0.9f), 96.0f };
    const material chrome   { glm::vec3(0.25f), glm::vec3(0.4f), glm::vec3(0.774f), 76.8f };
}

void setBool(const unsigned int& shaderProgram, const char* target, const bool& value);

void setInt(const unsigned int &shaderProgram, const char* target, const int &value);

void setFloat(const unsigned int &shaderProgram, const char* target, const float &value);

void setVec3(const unsigned int &shaderProgram, const char* target, const glm::vec3 &vector);

void setMat3(const unsigned int &shaderProgram, const char* target, const glm::mat3 &matrix);

void setMat4(const unsigned int &shaderProgram, const char* target, const glm::mat4 &matrix);

void setMaterial(const unsigned int& shaderProgram, const std::string &target);

void setMaterial(const unsigned int& shaderProgram, const std::string& target, const material& value);

void printVec3(const glm::vec3& vector);
