#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <engine.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WIN_W 800
#define WIN_H 600

using namespace std;

//-----------------------------------------------------------------------------------------------//

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

GLFWimage* load_image(const char* path){

    GLFWimage* img = new GLFWimage();
    img->pixels = stbi_load(path,&img->width,&img->height,nullptr,4);
    return img;
}

void input_handler(GLFWwindow* window){

    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
}

//-----------------------------------------------------------------------------------------------//

int main(){

    // INITIALIZATION -----------------------------------------------------------------//
    
    if(!glfwInit()){
        cerr << "Unable to Initialize GLFW!" << endl;
        return -1;
    }

    // OpenGL Version Configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MONITOR ------------------------------------------------------------------------//

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    
    if(!monitor){
        cerr << "Unable to fetch the Monitor!" << endl;
        return -1;
    }

    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

    // WINDOW -------------------------------------------------------------------------//

    GLFWwindow* window = glfwCreateWindow(WIN_W,WIN_H,"FirstWindow 1.0", nullptr, nullptr);

    if(!window){
        cerr << "Unable to initialize the window!" << endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL()){
        cerr << "Unable to Load OpenGL!" << endl;
        return -1;
    }
    
    int win_xpos = videoMode->width/2 - WIN_W/2;
    int win_ypos = videoMode->height/2 - WIN_H/2;

    GLFWimage* windowIcon = load_image("window_icon.png");
    
    glfwSetWindowPos(window,win_xpos,win_ypos);
    glfwSetWindowIcon(window,1,windowIcon);
    
    glfwSetWindowAttrib(window,GLFW_RESIZABLE,GLFW_FALSE);

    // MISC ---------------------------------------------------------------------------//

    glViewport(0,0,WIN_W,WIN_H);
    glClearColor(0.41f,0.35f,0.8f,1.0f);
    
    bool PAUSE = false;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Vertex Input //

    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex Shader //

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        cout << "ERROR: SHADER COMPILATION FAILED!\n" << infoLog << endl;
    }

    // Fragment Shader //

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Shader Program //

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        cout << "ERROR: SHADER-PROGRAM LINKING FAILED!\n" << infoLog << endl;
    }

    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window)){

        // Inputs //
        input_handler(window);

        // Updates //

        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Events //
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // TERMINATION --------------------------------------------------------------------//

    stbi_image_free(windowIcon->pixels);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}