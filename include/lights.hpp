#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shapes.hpp>

struct directionalLight{

    glm::vec3 direction;
    glm::vec3 color;
};

struct pointLight{

    glm::vec3 position;
    glm::vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct spotLight{

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;

    bool isVisible;

    float cutOffangle;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct lights{

    pointLight cubelight;
    directionalLight sunlight;
    spotLight flashlight;

    lights();
    static lights& instance();

    void update();
};

void setPointLight(const unsigned int &shaderProgram, const std::string &target, const pointLight &pl);

void setSpotLight(const unsigned int &shaderProgram, const std::string &target, const spotLight &sl);

void setDirectionalLight(const unsigned int &shaderProgram, const std::string &target, const directionalLight &dl);

struct colors {

    glm::vec3 red;
    glm::vec3 orange;
    glm::vec3 yellow;
    glm::vec3 green;
    glm::vec3 blue;
    glm::vec3 pink;
    glm::vec3 white;
    glm::vec3 black;
    glm::vec3 gray;

    colors();
    static colors& instance();
};

class lightSource {

    pointLight src;
    shape srcShape;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

public:

    lightSource(
        const glm::mat4& projection,
        const glm::mat4& view,
        const glm::mat4& model
    );

    void setLightColor(const glm::vec3& color);
    void setPosition(const glm::vec3& posiition);

    glm::vec3 getLightColor() const { return src.color; }
    const pointLight& getLight() const { return src; }

    void update(
        const glm::mat4& projection,
        const glm::mat4& view,
        const glm::mat4& model
    );

    void draw(const unsigned int& shader) const;
};

#endif