#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <shader.hpp>
#include <models.hpp>

// Hardcoded Models
struct shape {

    unsigned int VBO, VAO, EBO;
    unsigned int indicesCount;

    texture shapeTexture;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;

    ~shape();

    void bindVertices(
        const float* vertices, const size_t& size_v,
        const unsigned int* indices, const size_t& size_i
    );

    void bindVertices2D(
        const float* vertices, const size_t& size_v,
        const unsigned int* indices, const size_t& size_i
    );

    void loadTexture(const char* path);

    void update(
        const glm::mat4& projection,
        const glm::mat4& view,
        const glm::mat4& model
    );

    void draw(const unsigned int& shader) const;
};

struct shapes {

    shape square;
    shape cube;

    shapes();
    static shapes& instance();
};

#endif