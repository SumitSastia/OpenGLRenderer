#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WIN_W 800
#define WIN_H 600

using namespace std;

void iconify_callback(GLFWwindow* window, int iconified){

    if(iconified){ cout << "The window is minized." << endl; }
    else{ cout << "The window is restored." << endl; }
}

void iconify_requests(GLFWwindow* window, bool& PAUSE){

    if(!glfwGetWindowAttrib(window,GLFW_ICONIFIED)){ PAUSE = false; return; }

    if(glfwGetWindowAttrib(window,GLFW_ICONIFIED) && !PAUSE){
        glfwRequestWindowAttention(window); PAUSE = true;
    }
}

GLFWimage* load_image(const char* path){

    GLFWimage* img = new GLFWimage();
    img->pixels = stbi_load(path,&img->width,&img->height,nullptr,4);
    return img;
}

int main(){

    // INITIALIZATION -----------------------------------------------------------------//
    
    if(!glfwInit()){
        cerr << "Unable to Initialize GLFW!" << endl;
        return -1;
    }

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

    GLFWimage* windowIcon = load_image("..\\assets\\icons\\window_icon.png");
    
    glfwSetWindowPos(window,win_xpos,win_ypos);
    glfwSetWindowIcon(window,1,windowIcon);
    
    glfwSetWindowAttrib(window,GLFW_RESIZABLE,GLFW_FALSE);
    glfwSetWindowIconifyCallback(window,iconify_callback);

    bool PAUSE = false;

    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window)){

        // Updates //

        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        
        // Events //
        glfwPollEvents();
        // iconify_requests(window,PAUSE);
    }

    // TERMINATION --------------------------------------------------------------------//

    stbi_image_free(windowIcon->pixels);
    glfwTerminate();
    
    return 0;
}