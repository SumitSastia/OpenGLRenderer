#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIN_W 800
#define WIN_H 600

using namespace std;

//-----------------------------------------------------------------------------------------------//

float balanceVal = 0.5;

//-----------------------------------------------------------------------------------------------//

GLFWimage* load_image(const char* path){
    
    GLFWimage* img = new GLFWimage();
    img->pixels = stbi_load(path,&img->width,&img->height,nullptr,4);
    return img;
}

void input_handler(GLFWwindow* window){
    
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }
    
    if(glfwGetKey(window,GLFW_KEY_UP)){
        if(balanceVal < 1.0) balanceVal += 0.01;
    }
    
    if(glfwGetKey(window,GLFW_KEY_DOWN)){
        if(balanceVal > 0.0) balanceVal -= 0.01;
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
    glClearColor(0.13f,0.0f,0.2f,1.0f);
    
    glfwSwapInterval(1);
    
    // BUFFER -------------------------------------------------------------------------//
    
    float vertices[] = {
        
        // Position       // Color        // TextureCords
        -0.5f,0.5f,0.0f,   1.0f,0.0f,0.0f, 0.0f, 0.0f,
        -0.5f,-0.5f,0.0f, 0.0f,1.0f,0.0f, 0.0f, 1.0f,
        0.5f,0.5f,0.0f,  0.0f,0.0f,1.0f, 1.0f, 0.0f,
        0.5f,-0.5f,0.0f,  0.0f,0.0f,1.0f, 1.0f, 1.0f
    };
    
    unsigned int indices[] = {
        0,1,2,
        1,2,3
    };
    
    shader* s1 = new shader();
    
    buffer b1;
    b1.init(vertices, sizeof(vertices), indices, sizeof(indices));
    
    unsigned int VAO = b1.get_VAO();
    unsigned int shaderProgram = s1->get_program();
    
    // Textures -----------------------------------------------------------------------//
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    texture t1;
    texture t2;
    
    t1.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\wooden.jpg");
    t2.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\ceramic_stones.jpg");
    
    // GL MATHEMATICS -----------------------------------------------------------------//
    
    glm::mat4 trans = glm::mat4(1.0f);
    
    // Translation
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f,0.0f,1.0f));
    // trans = glm::translate(trans, glm::vec3(0.0f,1.0f,0.0f));
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    
    bool isRunning = true;
    
    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window) && isRunning){

        // Inputs //
        input_handler(window);

        // Updates //

        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        
        // Drawing using VAO //
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES,0,6);
        
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t1.getID());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, t2.getID());

        glUniform1i(glGetUniformLocation(shaderProgram,"ourTexture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram,"ourTexture2"), 1);
        
        glUniform1f(glGetUniformLocation(shaderProgram,"balance"), balanceVal);
        
        trans = glm::rotate(trans, (float)(sin(glfwGetTime())/5.0), glm::vec3(0.0f,0.0f,1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"transform"), 1, GL_FALSE, glm::value_ptr(trans));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glm::mat4 trans2 = glm::mat4(1.0f);
        // // trans = glm::translate(trans, glm::vec3(0.1f,0.1f,0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"transform"), 1, GL_FALSE, glm::value_ptr(trans2));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Events //
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // TERMINATION --------------------------------------------------------------------//

    b1.destroy();
    s1->destroy();
    
    stbi_image_free(windowIcon->pixels);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
