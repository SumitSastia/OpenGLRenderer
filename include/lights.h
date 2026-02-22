#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shapes.h>

namespace lights {

    struct directionalLight {

        glm::vec3 direction;
        glm::vec3 color;
    };

    struct pointLight {

        glm::vec3 position;
        glm::vec3 color;

        float constant;
        float linear;
        float quadratic;
    };

    struct spotLight {

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

    struct lights {

        pointLight cubelight;
        directionalLight sunlight;
        spotLight flashlight;

        lights();
        static lights& instance();

        void update();
    };

    void setPointLight(const unsigned int& shaderProgram, const std::string& target, const pointLight& pl);

    void setSpotLight(const unsigned int& shaderProgram, const std::string& target, const spotLight& sl);

    void setDirectionalLight(const unsigned int& shaderProgram, const std::string& target, const directionalLight& dl);

    class lightSource {

        pointLight src;
        specShape srcShape;

        glm::mat4 model;

    public:

        lightSource(const glm::mat4& model);

        void setLightColor(const glm::vec3& color);
        void setPosition(const glm::vec3& position);

        const pointLight& getLight() const { return src; }

        glm::vec3 getPosition() const { return src.position; }
        glm::vec3 getLightColor() const { return src.color; }
        glm::mat4 getModel() const { return model; }

        void update(const glm::mat4& model);
        void draw(const unsigned int& shader) const;
    };
}

namespace colors {

    const glm::vec3 red    { 1.0f, 0.0f, 0.0f };
    const glm::vec3 orange { 0.945f, 0.352f, 0.133f };
    const glm::vec3 yellow { 1.0f, 1.0f, 0.0f };
    const glm::vec3 green  { 0.0f, 1.0f, 0.0f };
    const glm::vec3 blue   { 0.0f, 0.0f, 1.0f };
    const glm::vec3 pink   { 0.89f, 0.239f, 0.58f };
    const glm::vec3 white  { 1.0f, 1.0f, 1.0f };
    const glm::vec3 black  { 0.0f, 0.0f, 0.0f };
    const glm::vec3 gray   { 0.392f, 0.392f, 0.392f };
}