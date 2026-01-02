#include <model_loader.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

//-------------------------------------------------------------------------------------//

mesh::mesh(const char* path){

    readFile(path);
    setupMesh();
}

void mesh::readFile(const char* path){

    std::ifstream file(path);

    if(!file.is_open()){
        std::cerr << "ERROR :: FAILED TO OPEN THE FILE!" << std::endl;
        return;
    }

    std::vector <glm::vec3> positions;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec2> texCords;

    std::string line;

    while(std::getline(file, line)){

        std::stringstream dataStream(line);
        std::string prefix;
        dataStream >> prefix;

        if(prefix == "v"){

            glm::vec3 temp_pos;
            dataStream >> temp_pos.x >> temp_pos.y >> temp_pos.z;
            positions.push_back(temp_pos);
        }

        else if(prefix == "vn"){

            glm::vec3 temp_nrl;
            dataStream >> temp_nrl.x >> temp_nrl.y >> temp_nrl.z;
            normals.push_back(temp_nrl);
        }

        else if(prefix == "vt"){

            glm::vec2 temp_tex;
            dataStream >> temp_tex.x >> temp_tex.y;
            texCords.push_back(temp_tex);
        }

        else if(prefix == "f"){

            std::string vertexIndex;
            std::vector <std::string> vertexIndices;
            const unsigned int baseIndex = vertices.size();

            while(dataStream >> vertexIndex){
                vertexIndices.push_back(vertexIndex);
            }

            for(const std::string &vi : vertexIndices){

                std::stringstream vertexStream(vi);
                std::string posStr, nrlStr, texStr;

                std::getline(vertexStream, posStr, '/');
                std::getline(vertexStream, texStr, '/');
                std::getline(vertexStream, nrlStr, '/');

                const unsigned int posIndex = std::stoi(posStr) - 1;
                const unsigned int nrlIndex = std::stoi(nrlStr) - 1;
                const unsigned int texIndex = std::stoi(texStr) - 1;

                vertex temp_v(positions[posIndex], normals[nrlIndex], texCords[texIndex]);
                vertices.push_back(temp_v);
            }

            const unsigned int size = vertexIndices.size();
            for(size_t i=1; i+1 < size; i++){

                indices.push_back(baseIndex);
                indices.push_back(baseIndex+i);
                indices.push_back(baseIndex+i+1);
            }
        }
    }
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

void mesh::loadVertices(const float* vertices, const size_t &size){

    if(size % 8){
        std::cerr << "ERROR::Vertex-Array size mismatched!" << std::endl;
        return;
    }

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

    if(size % 3){
        std::cerr << "ERROR::Vertex-Array size mismatched!" << std::endl;
        return;
    }

    for(unsigned int i=0; i<size; i++){
        this->indices.push_back(indices[i]);
    }
}

void mesh::draw(const unsigned int &shaderProgram){

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

}

//-------------------------------------------------------------------------------------//