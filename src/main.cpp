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

//-----------------------------------------------------------------------------------------------//

GLFWimage* load_image(const char* path);

void input_callback(GLFWwindow* window, int key, int scancode, int action, int modes);
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
    glClearColor(0.13f,0.0f,0.2f,1.0f);
    
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, input_callback);
    
    // BUFFER -------------------------------------------------------------------------//
    
    // float vertices[] = {

    //     // Front
    //     -0.5, 0.5, 0.5,  1.0,1.0,1.0,  0.0,0.0,
    //      0.5, 0.5, 0.5,  1.0,1.0,1.0,  1.0,0.0,
    //     -0.5,-0.5, 0.5,  1.0,1.0,1.0,  0.0,1.0,
    //      0.5,-0.5, 0.5,  1.0,1.0,1.0,  1.0,1.0,
         
    //     // Back
    //     -0.5, 0.5,-0.5,  1.0,1.0,1.0,  1.0,0.0,
    //      0.5, 0.5,-0.5,  1.0,1.0,1.0,  0.0,0.0,
    //     -0.5,-0.5,-0.5,  1.0,1.0,1.0,  1.0,1.0,
    //      0.5,-0.5,-0.5,  1.0,1.0,1.0,  0.0,1.0,

    //     // Left
    //     -0.5, 0.5,-0.5,  1.0,1.0,1.0,  0.0,0.0,
    //     -0.5, 0.5, 0.5,  1.0,1.0,1.0,  1.0,0.0,
    //     -0.5,-0.5,-0.5,  1.0,1.0,1.0,  0.0,1.0,
    //     -0.5,-0.5, 0.5,  1.0,1.0,1.0,  1.0,1.0,

    //     // Right
    //      0.5, 0.5,-0.5,  1.0,1.0,1.0,  1.0,0.0,
    //      0.5, 0.5, 0.5,  1.0,1.0,1.0,  0.0,0.0,
    //      0.5,-0.5,-0.5,  1.0,1.0,1.0,  1.0,1.0,
    //      0.5,-0.5, 0.5,  1.0,1.0,1.0,  0.0,1.0,

    //     // Top
    //     -0.5, 0.5,-0.5,  1.0,1.0,1.0,  0.0,0.0,
    //      0.5, 0.5,-0.5,  1.0,1.0,1.0,  1.0,0.0,
    //     -0.5, 0.5, 0.5,  1.0,1.0,1.0,  0.0,1.0,
    //      0.5, 0.5, 0.5,  1.0,1.0,1.0,  1.0,1.0,

    //     // Bottom
    //     -0.5,-0.5,-0.5,  1.0,1.0,1.0,  1.0,0.0,
    //      0.5,-0.5,-0.5,  1.0,1.0,1.0,  0.0,0.0,
    //     -0.5,-0.5, 0.5,  1.0,1.0,1.0,  1.0,1.0,
    //      0.5,-0.5, 0.5,  1.0,1.0,1.0,  0.0,1.0,
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
    
    // unsigned int indices[] = {

    //     // Front
    //     0,1,2,
    //     1,2,3,

    //     // Back
    //     4,5,6,
    //     5,6,7,

    //     // Left
    //     8,9,10,
    //     9,10,11,

    //     // Right
    //     12,13,14,
    //     13,14,15,

    //     // Top
    //     16,17,18,
    //     17,18,19,

    //     // Bottom
    //     20,21,22,
    //     21,22,23
    // };

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
    
    float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    texture t1;
    texture t2;

    t1.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\500-note-front.png");
    t2.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\500-note-back.png");
    
    // CAMERA -------------------------------------------------------------------------//
    
    camera cam;
    const float radius = 5.0f;
    
    // GL MATHEMATICS -----------------------------------------------------------------//
    
    glm::mat4 model(1.0f);
    glm::mat4 view;
    glm::mat4 projection;

    projection = cam.getPerspective();

    // LOOP CONTROLLERS ---------------------------------------------------------------//

    bool isRunning = true;
    glUseProgram(shaderProgram);

    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window) && isRunning){

        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Updates //
        if(!isPaused){
    
            // Inputs
            // track_mouse(window);
            cam.input_handler(window,deltaTime);
            cam.mouse_handler(window);

            float rotation_speed = 0.5f;
            float angle = 0.0f;

            if(cursor_dx != 0){
                angle = rotation_speed * cursor_dx;
                model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0,1.0,0.0)) * model;
            }

            if(cursor_dy != 0){
                angle = rotation_speed * cursor_dy;
                model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0,0.0,0.0)) * model;
            }

            float camX = sin(glfwGetTime()) * radius;
            float camZ = cos(glfwGetTime()) * radius;
            
            // cam.set_target(glm::vec3(cam.getPos().x,cam.getPos().y,cam.getPos().z - 5));
            cam.look_at();

            view = cam.getView();
        }
        
        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUniform1i(glGetUniformLocation(shaderProgram,"useTexture"), 0);
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO);

        // 3D Cube
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));

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