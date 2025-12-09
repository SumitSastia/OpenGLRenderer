#include <iostream>
#include <GLFW/glfw3.h>

using namespace std;

int main(){
    
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800,600,"FirstWindow 1.0", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)){

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}