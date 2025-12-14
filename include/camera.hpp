#ifndef CAMERA_HPP
#define CAMERA_HPP

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

public:

    camera();

    void look_at();
    void set_position(glm::vec3 position);
    void set_target(glm::vec3 target);
    glm::mat4 getView() const { return viewMatrix; }
};

#endif