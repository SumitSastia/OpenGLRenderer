#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <shader.hpp>
#include <models.hpp>

struct shape {

    unsigned int VBO, VAO, EBO;
    unsigned int indicesCount;

    texture shapeTexture;

    void bindVertices(
        const float* vertices, const size_t& size_v,
        const unsigned int* indices, const size_t& size_i
    );

    void loadTexture(const char* path);
    void draw(const unsigned int& shader) const;
};

struct shapes {

    shape cube;
    shape cylinder;
    shape sphere;

    shapes();
    static shapes& instance();
};

#endif