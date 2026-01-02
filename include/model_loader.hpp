#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

struct vertex{

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCords;

    vertex(
        const glm::vec3 &position = glm::vec3(0.0f),
        const glm::vec3 &normal = glm::vec3(0.0f),
        const glm::vec2 &textureCords = glm::vec3(0.0f)
    ):
        position(position),
        normal(normal),
        textureCords(textureCords) {}
};

class mesh{

    unsigned int VBO, VAO, EBO;

    void readFile(const char* path);
    void setupMesh();

public:

    std::vector <vertex> vertices;
    std::vector <unsigned int> indices;

    mesh(const char* path);

    mesh(
        const std::vector<vertex> &vertices,
        const std::vector<unsigned int> &indices
    ):
        vertices(vertices),
        indices(indices) {}

    void loadVertices(const float* vertices, const size_t &size);
    void loadIndices(const unsigned int* indices, const size_t &size);

    void draw(const unsigned int &shaderProgram);
};

#endif