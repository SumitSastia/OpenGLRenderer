#include <models.h>
#include <shader.h>
#include <camera.h>
#include <lights.h>

#include <iostream>
#include <stb_image.h>

using namespace std;

void meshTexture::loadTexture(const char* path, const std::string& directory) {

    int width, height;
    checkPath = path;

    unsigned char* pixelData = stbi_load(path, &width, &height, nullptr, 4);

    if (!pixelData) {
        std::cerr << "Failed to Load Image!\n" << path << std::endl;
        return;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixelData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//-------------------------------------------------------------------------------------//

mesh::~mesh() {

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void mesh::setupMesh() {

    size_t size_v = sizeof(vertex) * vertices.size();
    size_t size_i = sizeof(unsigned int) * indices.size();

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_v, vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1);

    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textureCords));
    glEnableVertexAttribArray(2);
}

void mesh::draw(const unsigned int& shader) const {

    unsigned int diffuse_count = 1;
    unsigned int specular_count = 1;

    const unsigned int size = textures.size();

    for (unsigned int i = 0; i < size; i++) {

        glActiveTexture(GL_TEXTURE0 + i);

        std::string numStr;
        const std::string name = textures[i].type;

        if (name == "texture_diffuse") {
            numStr = std::to_string(diffuse_count++);
        }
        else if (name == "texture_specular") {
            numStr = std::to_string(specular_count++);
        }

        setInt(shader, (name + numStr).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

//-------------------------------------------------------------------------------------//

void model3D::loadModel(const std::string& path) {

    //cout << "loadmodel" << endl;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::cerr << "ERROR::ASSIMP :: Unable to Open the Model File!" << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void model3D::processNode(aiNode* node, const aiScene* scene) {

    //cout << "processnode" << endl;

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {

        aiMesh* mesh_part = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh_part, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

mesh model3D::processMesh(aiMesh* _mesh, const aiScene* scene) {

    //cout << "processmesh" << endl;
    
    std::vector <vertex> vertices;
    std::vector <unsigned int> indices;
    std::vector <meshTexture> textures;

    for (unsigned int i = 0; i < _mesh->mNumVertices; i++) {

        vertex temp_vertex{};

        temp_vertex.position = glm::vec3(
            _mesh->mVertices[i].x,
            _mesh->mVertices[i].y,
            _mesh->mVertices[i].z
        );

        if (_mesh->HasNormals()) {

            temp_vertex.normal = glm::vec3(
                _mesh->mNormals[i].x,
                _mesh->mNormals[i].y,
                _mesh->mNormals[i].z
            );
        }

        if (_mesh->mTextureCoords[0]) {

            temp_vertex.textureCords = glm::vec2(
                _mesh->mTextureCoords[0][i].x,
                _mesh->mTextureCoords[0][i].y
            );
        }
        else {
            temp_vertex.textureCords = glm::vec2(0.0f);
        }

        vertices.push_back(temp_vertex);
    }

    for (unsigned int i = 0; i < _mesh->mNumFaces; i++) {

        aiFace face = _mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {

            indices.push_back(face.mIndices[j]);
        }
    }

    if (_mesh->mMaterialIndex >= 0) {
        
        aiMaterial* material = scene->mMaterials[_mesh->mMaterialIndex];

        std::vector <meshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector <meshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
    return mesh(vertices, indices, textures);
}

std::vector<meshTexture> model3D::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_str) const {

    std::vector <meshTexture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (const auto& tex : loadedTextures) {

            if (std::strcmp(tex.checkPath.data(), str.C_Str()) == 0) {

                textures.push_back(tex);
                skip = true; break;
            }
        }

        if (!skip) {

            meshTexture tex;
            tex.loadTexture(str.C_Str(), directory);
            tex.type = type_str;
            textures.push_back(tex);
        }
    }
    return textures;
}

void model3D::update(
    const glm::mat4& projection,
    const glm::mat4& view,
    const glm::mat4& model
) {
    this->projection = projection;
    this->view = view;
    this->model = model;
}

void model3D::draw(const unsigned int& shader) const {

    glUseProgram(shader);
    glFrontFace(GL_CW);

    setMat4(shader, "finalMatrix", projection * view * model);
    setMat4(shader, "model", model);
    setMat3(shader, "normalModel", glm::transpose(glm::inverse(glm::mat3(model))));
    setVec3(shader, "viewPos", camera::instance().getPos());

    setMaterial(shader, "m1");
    setSpotLight(shader, "s1", lights::instance().flashlight);

    for (const auto& mesh_part : meshes) {
        mesh_part.draw(shader);
    }

    glFrontFace(GL_CCW);
}
