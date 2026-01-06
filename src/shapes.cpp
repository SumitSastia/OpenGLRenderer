#include <shapes.hpp>
#include <camera.hpp>
#include <lights.hpp>
#include <iostream>

void shape::bindVertices(
    const float* vertices, const size_t& size_v,
    const unsigned int* indices, const size_t& size_i
) {
    indicesCount = size_i / sizeof(float);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_v, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TextureCords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void shape::loadTexture(const char* path) {
    shapeTexture.load(path);
}

void shape::update(
    const glm::mat4& projection,
    const glm::mat4& view,
    const glm::mat4& model
) {
    this->projection = projection;
    this->view = view;
    this->model = model;
}

void shape::draw(const unsigned int& shader) const {

    glUseProgram(shader);

    setMat4(shader, "finalMatrix", projection * view * model);
    setMat4(shader, "model", model);
    setMat3(shader, "normalModel", glm::transpose(glm::inverse(glm::mat3(model))));
    setVec3(shader, "viewPos", camera::instance().getPos());

    setMaterial(shader, "m1");
    setSpotLight(shader, "s1", lights::instance().flashlight);

    setInt(shader, "texture1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shapeTexture.getID());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//-------------------------------------------------------------------------------------//

shapes& shapes::instance() {
    static shapes instance;
    return instance;
}

shapes::shapes(){

    float vertices[] = {

        // Front
        -0.5, 0.5, 0.5,  0.0,0.0, 1.0, 0.0,0.0,
         0.5, 0.5, 0.5,  0.0,0.0, 1.0, 1.0,0.0,
        -0.5,-0.5, 0.5,  0.0,0.0, 1.0, 0.0,1.0,
         0.5,-0.5, 0.5,  0.0,0.0, 1.0, 1.0,1.0,
         
        // Back
         0.5, 0.5,-0.5,  0.0,0.0,-1.0, 0.0,0.0,
        -0.5, 0.5,-0.5,  0.0,0.0,-1.0, 1.0,0.0,
         0.5,-0.5,-0.5,  0.0,0.0,-1.0, 0.0,1.0,
        -0.5,-0.5,-0.5,  0.0,0.0,-1.0, 1.0,1.0,

        // Left
        -0.5, 0.5,-0.5,  -1.0,0.0,0.0, 0.0,0.0,
        -0.5, 0.5, 0.5,  -1.0,0.0,0.0, 1.0,0.0,
        -0.5,-0.5,-0.5,  -1.0,0.0,0.0, 0.0,1.0,
        -0.5,-0.5, 0.5,  -1.0,0.0,0.0, 1.0,1.0,

        // Right
         0.5, 0.5, 0.5,   1.0,0.0,0.0, 0.0,0.0,
         0.5, 0.5,-0.5,   1.0,0.0,0.0, 1.0,0.0,
         0.5,-0.5, 0.5,   1.0,0.0,0.0, 0.0,1.0,
         0.5,-0.5,-0.5,   1.0,0.0,0.0, 1.0,1.0,

        // Top
        -0.5, 0.5,-0.5,  0.0, 1.0,0.0, 0.0,0.0,
         0.5, 0.5,-0.5,  0.0, 1.0,0.0, 1.0,0.0,
        -0.5, 0.5, 0.5,  0.0, 1.0,0.0, 0.0,1.0,
         0.5, 0.5, 0.5,  0.0, 1.0,0.0, 1.0,1.0,

        // Bottom
         0.5,-0.5,-0.5,  0.0,-1.0,0.0, 0.0,0.0,
        -0.5,-0.5,-0.5,  0.0,-1.0,0.0, 1.0,0.0,
         0.5,-0.5, 0.5,  0.0,-1.0,0.0, 0.0,1.0,
        -0.5,-0.5, 0.5,  0.0,-1.0,0.0, 1.0,1.0
    };

    unsigned int indices[] = {

        // Front
        0,1,2,
        1,2,3,

        // Back
        4,5,6,
        5,6,7,

        // Left
        8,9,10,
        9,10,11,

        // Right
        12,13,14,
        13,14,15,

        // Top
        16,17,18,
        17,18,19,

        // Bottom
        20,21,22,
        21,22,23
    };

    cube.bindVertices(vertices, sizeof(vertices), indices, sizeof(indices));
    cube.loadTexture("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/wood_box.png");
}
