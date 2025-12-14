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

    // float vertices[] = {
        
    //     // Position       // Color        // TextureCords
    //     -0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f, 1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f, 1.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f, 1.0f, 1.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f, 0.0f, 0.0f,

    //     -0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f, 0.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f, 1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f, 1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f, 1.0f, 1.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f, 0.0f, 0.0f,

    //     -0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f, 1.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  0.0f,0.0f,1.0f, 1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f, 0.0f, 1.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f, 0.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f, 1.0f, 0.0f,

    //     0.5f,  0.5f,  0.5f,  1.0f,1.0f,0.0f, 1.0f, 0.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f,1.0f,0.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,1.0f,0.0f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  1.0f,1.0f,0.0f, 0.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  1.0f,1.0f,0.0f, 0.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f,1.0f,0.0f, 1.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f,  0.0f,1.0f,1.0f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f,  0.0f,1.0f,1.0f, 1.0f, 1.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f,1.0f,1.0f, 1.0f, 0.0f,
    //     0.5f, -0.5f,  0.5f,  0.0f,1.0f,1.0f, 1.0f, 0.0f,
    //     -0.5f, -0.5f,  0.5f,  0.0f,1.0f,1.0f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f,  0.0f,1.0f,1.0f, 0.0f, 1.0f,

    //     -0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f, 0.0f, 1.0f,
    //     0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f, 1.0f, 1.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f,0.0f,1.0f, 1.0f, 0.0f,
    //     0.5f,  0.5f,  0.5f,  1.0f,0.0f,1.0f, 1.0f, 0.0f,
    //     -0.5f,  0.5f,  0.5f,  1.0f,0.0f,1.0f, 0.0f, 0.0f,
    //     -0.5f,  0.5f, -0.5f,  1.0f,0.0f,1.0f, 0.0f, 1.0f
    // };
    
    // float vertices[] = {

    //     // Position //

    //     // Front
    //     -0.75, 0.45, 0.001,  1.0,1.0,1.0,  0.0,0.0,
    //      0.75, 0.45, 0.001,  1.0,1.0,1.0,  1.0,0.0,
    //     -0.75,-0.45, 0.001,  1.0,1.0,1.0,  0.0,1.0,
    //      0.75,-0.45, 0.001,  1.0,1.0,1.0,  1.0,1.0,
         
    //     // Back
    //     -0.75, 0.45,-0.001,  1.0,1.0,1.0,  0.0,0.0,
    //      0.75, 0.45,-0.001,  1.0,1.0,1.0,  1.0,0.0,
    //     -0.75,-0.45,-0.001,  1.0,1.0,1.0,  0.0,1.0,
    //      0.75,-0.45,-0.001,  1.0,1.0,1.0,  1.0,1.0
    // };

    float vertices[] = {

        // Position //

        // Front
        -0.5, 0.5, 0.5,  1.0,0.0,0.0,  0.0,0.0,
         0.5, 0.5, 0.5,  0.0,1.0,0.0,  1.0,0.0,
        -0.5,-0.5, 0.5,  0.0,0.0,1.0,  0.0,1.0,
         0.5,-0.5, 0.5,  1.0,1.0,0.0,  1.0,1.0,
         
        // Back
        -0.5, 0.5,-0.5,  0.0,1.0,1.0,  0.0,0.0,
         0.5, 0.5,-0.5,  1.0,0.0,1.0,  1.0,0.0,
        -0.5,-0.5,-0.5,  0.5,1.0,0.5,  0.0,1.0,
         0.5,-0.5,-0.5,  1.0,1.0,1.0,  1.0,1.0
    };
    
    unsigned int indices[] = {

        // Front
        0,1,2,
        1,2,3,

        // Back
        4,5,6,
        5,6,7,

        // Left
        4,0,6,
        0,6,2,

        // Right
        5,1,7,
        1,7,3,

        // Top
        4,5,0,
        5,0,1,

        // Bottom
        6,7,2,
        7,2,3
    };
    
    shader* s1 = new shader();
    
    buffer b1;
    b1.init(vertices, sizeof(vertices), indices, sizeof(indices));
    
    unsigned int VAO = b1.get_VAO();
    unsigned int shaderProgram = s1->get_program();
    
    // Textures -----------------------------------------------------------------------//
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    
    // float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // texture t1;
    // t1.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\500-note-front.png");
    
    // GL MATHEMATICS -----------------------------------------------------------------//
    
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection;

    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));
    view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
    projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    bool isRunning = true;
    
    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window) && isRunning){

        // Inputs //
        input_handler(window);

        // Updates //
        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  

        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, t1.getID());

        // glUniform1i(glGetUniformLocation(shaderProgram,"ourTexture1"), 0);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
