#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <shader.h>
#include <lights.h>

//-------------------------------------------------------------------------------------//

shader::shader(const char* vertPath, const char* fragPath){

    // Vertex & Fragment Shader //

    std::string vertexStr = loadShader(vertPath);
    std::string fragmentStr = loadShader(fragPath);

    const char* vertexShaderSource = vertexStr.c_str();
    const char* fragmentShaderSource = fragmentStr.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR: VERTEX-SHADER COMPILATION FAILED!\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR: FRAGMENT-SHADER COMPILATION FAILED!\n" << infoLog << std::endl;
    }

    // Shader Program //
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR: SHADER-PROGRAM LINKING FAILED!\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string shader::loadShader(const char* path){

    std::ifstream file(path);

    if(!file){
        std::cerr << "Failed to open the File!" << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

//-------------------------------------------------------------------------------------//

line::~line(){
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
}

void line::initLines(const glm::vec3 startPos, const glm::vec3 endPos){

    vertices[0] = startPos.x;
    vertices[1] = startPos.y;
    vertices[2] = startPos.z;

    vertices[3] = endPos.x;
    vertices[4] = endPos.y;
    vertices[5] = endPos.z;
    
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void line::updateLines(const glm::vec3 startPos, const glm::vec3 endPos){

    vertices[0] = startPos.x;
    vertices[1] = startPos.y;
    vertices[2] = startPos.z;

    vertices[3] = endPos.x;
    vertices[4] = endPos.y;
    vertices[5] = endPos.z;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

//-------------------------------------------------------------------------------------//

texture::~texture() {
    glDeleteTextures(1, &textureID);
}

void texture::load(const char* path){

    pixelData = stbi_load(path, &width, &height, nullptr, 4);

    if(!pixelData){
        std::cerr << "Failed to Load Image!\n" << path << std::endl;
        return;
    }

    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

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

frame_buffer::frame_buffer(const int& frameWidth, const int& frameHeight) {

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Texture Attachment
    glGenTextures(1, &frameTexture);
    glBindTexture(GL_TEXTURE_2D, frameTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture, 0);

    // Render Object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR :: UNABLE TO COMPLETE FRAME BUFFER!" << std::endl;
    }
    else {

        // Vertices Binding
        const float vertices[] = {

            // Position  // Cords
            -1.0f, 1.0f, 0.0f,1.0f,
             1.0f, 1.0f, 1.0f,1.0f,
            -1.0f,-1.0f, 0.0f,0.0f,

             1.0f, 1.0f, 1.0f,1.0f,
             1.0f,-1.0f, 1.0f,0.0f,
            -1.0f,-1.0f, 0.0f,0.0f
        };

        const unsigned int indices[] = {
            0,1,2,
            1,3,2
        };

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        //std::cout << "VBO: " << VBO << ", VAO: " << VAO << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

frame_buffer::~frame_buffer() {

    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &RBO);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

//-------------------------------------------------------------------------------------//

materials& materials::instance(){
    static materials instance;
    return instance;
}

materials::materials(){

    rubber.ambient = glm::vec3(0.02f);
    rubber.diffuse = glm::vec3(0.01f);
    rubber.specular = glm::vec3(0.4f);
    rubber.shininess = 10.0f;

    wood.ambient = glm::vec3(0.2f, 0.15f, 0.1f);
    wood.diffuse = glm::vec3(0.4f, 0.3f, 0.2f);
    wood.specular = glm::vec3(0.05f);
    wood.shininess = 8.0f;

    plastic.ambient = glm::vec3(0.1f);
    plastic.diffuse = glm::vec3(0.6f);
    plastic.specular = glm::vec3(0.2f);
    plastic.shininess = 16.0f;

    concrete.ambient = glm::vec3(0.05f);
    concrete.diffuse = glm::vec3(0.5f);
    concrete.specular = glm::vec3(0.1f);
    concrete.shininess = 4.0f;

    glass.ambient = glm::vec3(0.0f);
    glass.diffuse = glm::vec3(0.3f);
    glass.specular = glm::vec3(0.9f);
    glass.shininess = 96.0f;

    chrome.ambient = glm::vec3(0.25f);
    chrome.diffuse = glm::vec3(0.4f);
    chrome.specular = glm::vec3(0.774f);
    chrome.shininess = 76.8f;
}

void setInt(const unsigned int &shaderProgram, const char* target, const int &value){

    glUniform1i(
        glGetUniformLocation(shaderProgram, target),
        value
    );
}

void setFloat(const unsigned int &shaderProgram, const char* target, const float &value){

    glUniform1f(
        glGetUniformLocation(shaderProgram, target),
        value
    );
}

void setVec3(const unsigned int &shaderProgram, const char* target, const glm::vec3 &vector){

    glUniform3fv(
        glGetUniformLocation(shaderProgram, target),
        1,
        glm::value_ptr(vector)
    );
}

void setMat3(const unsigned int &shaderProgram, const char* target, const glm::mat3 &matrix){

    glUniformMatrix3fv(
        glGetUniformLocation(shaderProgram, target),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    );
}

void setMat4(const unsigned int &shaderProgram, const char* target, const glm::mat4 &matrix){

    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, target),
        1,
        GL_FALSE,
        glm::value_ptr(matrix)
    );
}

void setMaterial(const unsigned int& shaderProgram, const std::string &target){

    setVec3(shaderProgram, (target + ".ambient").c_str(), materials::instance().wood.ambient);
    setVec3(shaderProgram, (target + ".diffuse").c_str(), materials::instance().wood.diffuse);
    setVec3(shaderProgram, (target + ".specular").c_str(), materials::instance().wood.specular);
    setFloat(shaderProgram, (target + ".shininess").c_str(), materials::instance().glass.shininess);
}

void printVec3(const glm::vec3& vector) {
    std::cout << "x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << std::endl;
}