#include <camera.hpp>

camera::camera(){

    position = glm::vec3(0.0f, 0.0f, 3.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    direction = glm::normalize(position - direction);

    glm::vec3 up(0.0f, 1.0f, 0.0f);

    right_axis = glm::normalize(glm::cross(up, direction));
    up_axis = glm::cross(direction, right_axis);
}

void camera::set_position(glm::vec3 position){
    this->position = position;
}

void camera::set_target(glm::vec3 target){
    this->target = target;
}

void camera::look_at(){
    viewMatrix = glm::lookAt(position, target, up_axis);
}