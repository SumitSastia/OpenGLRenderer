#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <stb_image.h>

#include <camera.hpp>

#include <filesystem>
#include <cstdlib>

#define WIN_W 800
#define WIN_H 600

using namespace std;

//-----------------------------------------------------------------------------------------------//

float balanceVal = 0.5f;

bool isPaused = false;
bool mouseInCamera = false;

float lastTime = 0.0f;
float deltaTime = 0.0f;

double cursor_dx = 0.0;
double cursor_dy = 0.0;

float scrollOffset = 0.0f;

//-----------------------------------------------------------------------------------------------//

GLFWimage* load_image(const char* path);

void input_callback(GLFWwindow* window, int key, int scancode, int action, int modes);
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y);

void track_mouse(GLFWwindow* window);

void checkRestart(){

    if(filesystem::exists("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\restart.flag")){

        filesystem::remove("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\restart.flag");
        system("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\build\\Debug\\firstGame.exe");
        exit(0);
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
    glClearColor(0.13f,0.0f,0.2f,0.5f);
    
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, input_callback);
    
    // BUFFER -------------------------------------------------------------------------//

    float vertices[] = {

        // Front
        -0.5, 0.5, 0.5,  1.0,0.0,0.0,  0.0,0.0,1.0,
         0.5, 0.5, 0.5,  0.0,1.0,0.0,  0.0,0.0,1.0,
        -0.5,-0.5, 0.5,  0.0,0.0,1.0,  0.0,0.0,1.0,
         0.5,-0.5, 0.5,  1.0,1.0,0.0,  0.0,0.0,1.0,
         
        // Back
        -0.5, 0.5,-0.5,  0.0,1.0,1.0,  0.0,0.0,-1.0,
         0.5, 0.5,-0.5,  1.0,0.0,1.0,  0.0,0.0,-1.0,
        -0.5,-0.5,-0.5,  1.0,1.0,1.0,  0.0,0.0,-1.0,
         0.5,-0.5,-0.5,  0.5,1.0,0.5,  0.0,0.0,-1.0,

        // Left
        -0.5, 0.5,-0.5,  0.0,1.0,1.0,  -1.0,0.0,0.0,
        -0.5, 0.5, 0.5,  1.0,0.0,0.0,  -1.0,0.0,0.0,
        -0.5,-0.5,-0.5,  1.0,1.0,1.0,  -1.0,0.0,0.0,
        -0.5,-0.5, 0.5,  0.0,0.0,1.0,  -1.0,0.0,0.0,

        // Right
         0.5, 0.5,-0.5,  1.0,0.0,1.0,  1.0,0.0,0.0,
         0.5, 0.5, 0.5,  0.0,1.0,0.0,  1.0,0.0,0.0,
         0.5,-0.5,-0.5,  0.5,1.0,0.5,  1.0,0.0,0.0,
         0.5,-0.5, 0.5,  1.0,1.0,0.0,  1.0,0.0,0.0,

        // Top
        -0.5, 0.5,-0.5,  0.0,1.0,1.0,  0.0,1.0,0.0,
         0.5, 0.5,-0.5,  1.0,0.0,1.0,  0.0,1.0,0.0,
        -0.5, 0.5, 0.5,  1.0,0.0,0.0,  0.0,1.0,0.0,
         0.5, 0.5, 0.5,  0.0,1.0,0.0,  0.0,1.0,0.0,

        // Bottom
        -0.5,-0.5,-0.5,  1.0,1.0,1.0,  0.0,-1.0,0.0,
         0.5,-0.5,-0.5,  0.5,1.0,0.5,  0.0,-1.0,0.0,
        -0.5,-0.5, 0.5,  0.0,0.0,1.0,  0.0,-1.0,0.0,
         0.5,-0.5, 0.5,  1.0,1.0,0.0,  0.0,-1.0,0.0,
    };

    unsigned int indices[] = {

        // Front
        0,1,2,
        1,2,3,

        // Back
        4,5,6,
        5,6,7,

        // Left
        8,9,10,
        9,10,11,

        // Right
        12,13,14,
        13,14,15,

        // Top
        16,17,18,
        17,18,19,

        // Bottom
        20,21,22,
        21,22,23
    };

    float vertices2[] = {

        // Position //

        // Front
        -0.5, 0.5, 0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
         0.5, 0.5, 0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
        -0.5,-0.5, 0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
         0.5,-0.5, 0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
         
        // Back
        -0.5, 0.5,-0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
         0.5, 0.5,-0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
        -0.5,-0.5,-0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
         0.5,-0.5,-0.5,  1.0, 1.0, 1.0,  0.0, 0.0, 0.0,
    };
    
    unsigned int indices2[] = {

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
    
    shader* s1 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\basic.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\basic.frag"
    );

    shader* s2 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\light.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\light.frag"
    );
    
    buffer b1;
    buffer b2; // Light-Source Cube

    b1.init(vertices, sizeof(vertices), indices, sizeof(indices));
    b2.init(vertices2, sizeof(vertices2), indices2, sizeof(indices2));
    
    unsigned int VAO = b1.get_VAO();
    unsigned int VAO2 = b2.get_VAO();

    unsigned int shaderProgram = s1->get_program();
    unsigned int lightShader = s2->get_program();
    
    // Textures -----------------------------------------------------------------------//
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    
    // CAMERA -------------------------------------------------------------------------//
    
    camera cam;
    glfwSetScrollCallback(window, scroll_callback);
    
    // GL MATHEMATICS -----------------------------------------------------------------//
    
    glm::mat4 model(1.0f);
    glm::mat4 view;
    glm::mat4 projection;

    projection = cam.getPerspective();

    // LIGHTING -----------------------------------------------------------------------//

    glm::vec3 light(1.0f, 1.0f, 1.0f);
    glm::vec3 coral(1.0f, 0.5f, 0.31f);

    glm::vec3 lightPos(-3.0f, 1.5f, 3.0f);
    glm::mat4 lightModel(1.0f);
    
    lightModel = glm::translate(lightModel, lightPos);

    // LOOP CONTROLLERS ---------------------------------------------------------------//

    bool isRunning = true;

    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window) && isRunning){

        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Updates //
        if(!isPaused){
    
            // Inputs
            cam.input_handler(window,deltaTime);
            cam.mouse_handler(window);
            cam.scroll_handler(scrollOffset);
            
            cam.look_at();

            view = cam.getView();
            projection = cam.getPerspective();

            // Light-Rotation
            lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0,1.0,0.0)) * lightModel;

            lightPos = glm::vec3(-3.0f, 1.5f, 3.0f);
            lightPos = glm::vec3(lightModel * glm::vec4(lightPos, 1.0f));
        }
        
        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(light));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        
        // Light-Source
        glUseProgram(lightShader);
        glm::mat4 finalMatrix = projection * view * lightModel;
        
        glUniformMatrix4fv(
            glGetUniformLocation(lightShader, "finalMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(finalMatrix)
        );

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));

        // Cube - Object1
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "view"),
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(model)
        );
        glUniform3f(
            glGetUniformLocation(shaderProgram, "viewPos"),
            cam.getPos().x,
            cam.getPos().y,
            cam.getPos().z
        );

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));

        // Events //
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // TERMINATION --------------------------------------------------------------------//

    b1.destroy();
    b2.destroy();

    s1->destroy();
    s2->destroy();
    
    stbi_image_free(windowIcon->pixels);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

//-----------------------------------------------------------------------------------------------//

GLFWimage* load_image(const char* path){
    
    GLFWimage* img = new GLFWimage();
    img->pixels = stbi_load(path,&img->width,&img->height,nullptr,4);
    return img;
}

void input_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if(glfwGetKey(window,GLFW_KEY_ESCAPE)){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }

    if(glfwGetKey(window,GLFW_KEY_F)){
        isPaused = !isPaused;
    }

    if(glfwGetKey(window,GLFW_KEY_E)){

        if(!mouseInCamera){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        mouseInCamera = !mouseInCamera;
    }
}

void scroll_callback(GLFWwindow* window, double offset_x, double offset_y){
    scrollOffset = (float)offset_y;
}

void track_mouse(GLFWwindow* window){

    static bool mouse_click_hold = false;

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){

        cursor_dx = 0.0;
        cursor_dy = 0.0;
        mouse_click_hold = false;
        return;
    }

    double pos_x;
    double pos_y;

    glfwGetCursorPos(window, &pos_x, &pos_y);

    static double prev_x = pos_x;
    static double prev_y = pos_y;

    if(!mouse_click_hold){
        prev_x = pos_x;
        prev_y = pos_y;
        mouse_click_hold = true;
    }

    cursor_dx = pos_x - prev_x;
    cursor_dy = pos_y - prev_y;

    prev_x = pos_x;
    prev_y = pos_y;
}