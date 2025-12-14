#include <camera.hpp>

camera::camera(){

    position = glm::vec3(0.0f, 0.0f, 3.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    direction = glm::normalize(position - direction);

    camSpeed = 1.0f;
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    right_axis = glm::normalize(glm::cross(up, direction));
    up_axis = glm::cross(direction, right_axis);
}

void camera::set_position(const glm::vec3 position){
    this->position = position;
}

void camera::set_target(const glm::vec3 target){
    this->target = target;
}

void camera::set_speed(const float speed){
    camSpeed = speed;
}

void camera::look_at(){
    viewMatrix = glm::lookAt(position, target, up_axis);
}

void camera::input_handler(GLFWwindow* window, float deltaTime){

    if(glfwGetKey(window,GLFW_KEY_W)){
        position.z -= camSpeed * deltaTime;
    }
    if(glfwGetKey(window,GLFW_KEY_S)){
        position.z += camSpeed * deltaTime;
    }

    if(glfwGetKey(window,GLFW_KEY_A)){
        position.x -= camSpeed * deltaTime;
    }
    if(glfwGetKey(window,GLFW_KEY_D)){
        position.x += camSpeed * deltaTime;
    }

}