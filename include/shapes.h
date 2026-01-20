#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <shader.h>
#include <models.h>

// Colored Models
struct coloredCube {

    unsigned int VBO, VAO, EBO;
    unsigned int colorShader;

    glm::vec3 color;

    coloredCube(const glm::vec3& color);
    ~coloredCube();

    void update(const float& delta_time);
    void render() const;
};

struct shape2D {

    unsigned int VBO, VAO, EBO;
    unsigned int indicesCount;

    texture shape2DTexture;

    ~shape2D();

    void bindVertices(
        const float* vertices, const size_t& size_v,
        const unsigned int* indices, const size_t& size_i
    );

    void loadTexture(const char* texturePath);
    void draw(const unsigned int& shader, const glm::mat4& model) const;
    void drawShadow() const;
};

// Hardcoded Models
struct shape {

    unsigned int VBO, VAO, EBO;
    unsigned int indicesCount;

    texture shapeDiffuseTexture;
    texture shapeSpecularTexture;

    ~shape();

    void bindVertices(
        const float* vertices, const size_t& size_v,
        const unsigned int* indices, const size_t& size_i
    );

    void loadTexture(const char* diffusePath, const char* specularPath);
    void draw(const unsigned int& shader, const glm::mat4& model) const;
    void drawShadow() const;
};

struct shapeInstanced : shape {

    void bindVertices(
        const float* vertices, const size_t& size_v,
        const unsigned int* indices, const size_t& size_i
    );

    void draw(const unsigned int& shader, const unsigned int& instanceCounts) const;
    void drawShadow(const unsigned int& instanceCounts) const;
};

struct shapes {

    shape2D square;
    shape cube;
    shapeInstanced cubeInstanced;

    shapes();
    static shapes& instance();
};

class cubeMap {
    
    unsigned int VBO, VAO, textureID;

public:

    cubeMap(const std::vector <std::string>& textureFaces);
    ~cubeMap();

    const unsigned int& get_VAO() const { return VAO; }
    const unsigned int& get_ID() const { return textureID; }
};

#endif