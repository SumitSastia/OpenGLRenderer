#include <mesh.hpp>
#include <iostream>

vertex::vertex(
    const glm::vec3 &position,
    const glm::vec3 &normal,
    const glm::vec2 &textureCords
){

    this->position = position;
    this->normal = normal;
    this->textureCords = textureCords;
}

void mesh::setupMesh(){

    size_t size_v = sizeof(vertex) * vertices.size();
    size_t size_i = sizeof(unsigned int) * indices.size();

    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, size_v, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i, &indices[0], GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    glEnableVertexAttribArray(1);

    // Texture
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, textureCords));
    glEnableVertexAttribArray(2);
}

mesh::mesh(
    const std::vector<vertex> &vertices,
    const std::vector<unsigned int> &indices,
    const std::vector<texture> &textures
){

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void mesh::draw(const unsigned int &shaderProgram){

    unsigned int diffuseCount = 1;
    unsigned int specularCount = 1;

    for(unsigned int i=0; i<textures.size(); i++){

        glActiveTexture(GL_TEXTURE0 + i);
        std::string textureType = textures[0].getType();
        std::string count;
        
        if(textureType == "texture_diffuse"){
            count = std::to_string(diffuseCount++);
        }
        else if(textureType == "texture_specular"){
            count = std::to_string(specularCount++);
        }

        setInt(shaderProgram, ("material."+textureType+count).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].getID());
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void mesh::loadVertices(const float* vertices, const size_t &size){

    if(size % 8) return;

    unsigned int itr = 0;

    while(itr < size){

        vertex tempVertex(
            glm::vec3(vertices[itr++], vertices[itr++], vertices[itr++]),
            glm::vec3(vertices[itr++], vertices[itr++], vertices[itr++]),
            glm::vec2(vertices[itr++], vertices[itr++])
        );

        this->vertices.push_back(tempVertex);
    }
}

void mesh::loadIndices(const unsigned int* indices, const size_t &size){

    if(size % 3) return;

    for(unsigned int i=0; i<size; i++){
        this->indices.push_back(indices[i]);
    }
}

//-------------------------------------------------------------------------------------//

std::vector <texture> model3d::loadTextures(aiMaterial* mat, aiTextureType type, std::string typeName){

    std::vector <texture> textures;

    for(unsigned int i=0; i < mat->GetTextureCount(type); i++){

        aiString str;
        mat->GetTexture(type, i, &str);

        texture T;

        T.load(str.C_Str());
        T.type = typeName;
        T.filePath = str.C_Str();

        textures.push_back(T);
    }

    return textures;
}

mesh model3d::processMesh(aiMesh* modelMesh, const aiScene* scene){

    std::vector <vertex> vertices;
    std::vector <unsigned int> indices;
    std::vector <texture> textures;

    for(unsigned int i=0; i < modelMesh->mNumVertices; i++){

        // Vertex
        vertex v;

        v.position = glm::vec3(
            modelMesh->mVertices[i].x,
            modelMesh->mVertices[i].y,
            modelMesh->mVertices[i].z
        );

        v.normal = glm::vec3(
            modelMesh->mNormals[i].x,
            modelMesh->mNormals[i].y,
            modelMesh->mNormals[i].z
        );

        if(modelMesh->mTextureCoords[0]){

            v.textureCords = glm::vec2(
                modelMesh->mTextureCoords[0][i].x,
                modelMesh->mTextureCoords[0][i].y
            );
        }
        else{
            v.textureCords = glm::vec2(0.0f);
        }

        vertices.push_back(v);
    }

    // Index
    for(unsigned int i=0; i < modelMesh->mNumFaces; i++){

        const aiFace face = modelMesh->mFaces[i];

        for(unsigned int j=0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // Texture
    if(modelMesh->mMaterialIndex >= 0){

        aiMaterial* material = scene->mMaterials[ modelMesh->mMaterialIndex ];

        std::vector <texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector <texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return mesh(vertices, indices, textures);
}

void model3d::processNode(aiNode* node, const aiScene* scene){

    for(unsigned int i=0; i < node->mNumMeshes; i++){

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int j=0; j < node->mNumChildren; j++){
        processNode(node->mChildren[j], scene);
    }
}

void model3d::loadModel(const std::string &path){

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){

        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    modelPath = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    // std::cout << "didnt even reach here" << std::endl;
}

void model3d::draw(const unsigned int &shaderProgram){

    for(unsigned int i=0; i < meshes.size(); i++){
        meshes[i].draw(shaderProgram);
    }
}