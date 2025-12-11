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
    glClearColor(0.52f,0.0f,0.8f,0.8f);
    
    bool PAUSE = false;
    
    float vertices[] = {

        // Position       // Color
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Top-right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-left

        // 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f // Bottom-left
        // -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2,
        1,2,3
    };

    engine::get_instance()->init(vertices, sizeof(vertices), indices, sizeof(indices));

    unsigned int VAO = engine::get_instance()->get_VAO();
    unsigned int EBO = engine::get_instance()->get_EBO();

    unsigned int shaderProgram = engine::get_instance()->get_program();
    
    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window)){

        // Inputs //
        input_handler(window);

        // Updates //

        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES,0,6);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Events //
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // TERMINATION --------------------------------------------------------------------//

    engine::get_instance()->destroy();
    stbi_image_free(windowIcon->pixels);

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}