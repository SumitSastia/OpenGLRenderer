#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera{

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 target;

    glm::vec3 right_axis;
    glm::vec3 up_axis;

    glm::mat4 viewMatrix;

    float camSpeed;

public:

    camera();

    void look_at();
    void input_handler(GLFWwindow* window, float deltaTime);

    void set_speed(const float speed);
    void set_position(const glm::vec3 position);
    void set_target(const glm::vec3 target);

    glm::vec3 getPos() const { return position; }
    glm::mat4 getView() const { return viewMatrix; }
};

#endif