#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>

#include <shader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

struct vertex {

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCords;

    vertex(
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& normal = glm::vec3(0.0f),
        const glm::vec2& textureCords = glm::vec3(0.0f)
    ) :
        position(position),
        normal(normal),
        textureCords(textureCords) {
    }
};

struct meshTexture {

    unsigned int id;
    std::string type;
    void loadTexture(const char* path, const std::string& directory);
};

class mesh {

    unsigned int VBO, VAO, EBO;
    void setupMesh();

public:

    std::vector <vertex> vertices;
    std::vector <unsigned int> indices;
    std::vector <meshTexture> textures;

    mesh(
        const std::vector <vertex>& vertices,
        const std::vector <unsigned int>& indices,
        const std::vector <meshTexture>& textures
    ) :
        vertices(vertices),
        indices(indices),
        textures(textures),
        VBO(0), VAO(0), EBO(0) {

        setupMesh();
    }

    void draw(const unsigned int& shader) const ;
};

class model3D {

    std::vector <mesh> meshes;
    std::string directory;

    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);
    mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<meshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_str) const;

public:

    model3D(const char* path) {
        loadModel(path);
    }

    void draw(const unsigned int& shader) const;
};

#endif