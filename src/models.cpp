#include <models.hpp>
#include <shader.hpp>
#include <iostream>

#include <stb_image.h>

void meshTexture::loadTexture(const char* path, const std::string& directory) {

    int width, height;
    unsigned char* pixelData = stbi_load((directory + path).c_str(), &width, &height, nullptr, 4);

    if (!pixelData) {
        std::cerr << "Failed to Load Image!\n" << path << std::endl;
        return;
    }

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

//-------------------------------------------------------------------------------------//

void mesh::setupMesh() {

    size_t size_v = sizeof(vertex) * vertices.size();
    size_t size_i = sizeof(unsigned int) * indices.size();

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_v, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, &indices[0], GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1);

    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textureCords));
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
}

//-------------------------------------------------------------------------------------//

void model3D::loadModel(const std::string& path) {

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

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {

        aiMesh* mesh_part = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh_part, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

mesh model3D::processMesh(aiMesh* mesh, const aiScene* scene) {
    
    std::vector <vertex> vertices;
    std::vector <unsigned int> indices;
    std::vector <meshTexture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        vertex temp_vertex;

        temp_vertex.position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        temp_vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        if (mesh->mTextureCoords[0]) {

            temp_vertex.textureCords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else {
            temp_vertex.textureCords = glm::vec2(0.0f);
        }
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {

            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector <meshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector <meshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
}

std::vector<meshTexture> model3D::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_str) const {

    std::vector <meshTexture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        meshTexture tex;
        tex.loadTexture(str.C_Str(), directory);
        tex.type = type_str;
        textures.push_back(tex);
    }
    return textures;
}

void model3D::draw(const unsigned int& shader) const {

    for (const auto& mesh_part : meshes) {
        mesh_part.draw(shader);
    }
}
