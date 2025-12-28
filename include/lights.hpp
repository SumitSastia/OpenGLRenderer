#ifndef LIGHTS_HPP
#define LIGHTS_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
};

void setPointLight(const unsigned int &shaderProgram, const std::string &target, const pointLight &pl);

void setSpotLight(const unsigned int &shaderProgram, const std::string &target, const spotLight &sl);

void setDirectionalLight(const unsigned int &shaderProgram, const std::string &target, const directionalLight &dl);

#endif