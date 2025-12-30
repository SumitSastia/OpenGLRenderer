#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <shader.hpp>

struct vertex{

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCords;

    vertex() {}

    vertex(
        const glm::vec3 &position,
        const glm::vec3 &normal,
        const glm::vec2 &textureCords
    );
};

class mesh{

    unsigned int VBO, VAO, EBO;
    void setupMesh();

public:

    std::vector <vertex> vertices;
    std::vector <unsigned int> indices;
    std::vector <texture> textures;

    mesh() {}

    mesh(
        const std::vector<vertex> &vertices,
        const std::vector<unsigned int> &indices,
        const std::vector<texture> &textures
    );

    void draw(const unsigned int &shaderProgram);
    
    void loadVertices(const float* vertices, const size_t &size);
    void loadIndices(const unsigned int* indices, const size_t &size);
};

class model{

    std::vector <mesh> meshes;
    std::string modelPath;

    void loadModel(const std::string &path);

    void processNode(aiNode* node, const aiScene* scene);
    mesh processMesh(aiMesh* modelMesh, const aiScene* scene);

    std::vector <texture> loadTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:

    model(const char* path){
        loadModel(path);
    }

    void draw(const unsigned int &shaderProgram);
};

#endif