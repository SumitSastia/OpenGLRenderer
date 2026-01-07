#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader.hpp>
#include <camera.hpp>
#include <lights.hpp>
#include <models.hpp>
#include <shapes.hpp>
#include <scenes.hpp>

using namespace std;

//-----------------------------------------------------------------------------------------------//

int frameWidth = 0;
int frameHeight = 0;

float balanceVal = 0.5f;

bool isPaused = false;
bool mouseInCamera = false;

bool rotateCube = false;
bool showLines = false;

float lastTime = 0.0f;
float deltaTime = 0.0f;

double cursor_dx = 0.0;
double cursor_dy = 0.0;

float scrollOffset = 0.0f;

bool useFlashLight = true;

//-----------------------------------------------------------------------------------------------//

GLFWimage* load_image(const char* path);

void input_callback(GLFWwindow* window, int key, int scancode, int action, int modes);
void scroll_callback(GLFWwindow* window, double offset_x, double offset_y);

void track_mouse(GLFWwindow* window);

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

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    const unsigned int win_xpos = videoMode->width/2 - WIN_W/2;
    const unsigned int win_ypos = videoMode->height/2 - WIN_H/2;
    
    const GLFWimage* windowIcon = load_image("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/icons/window_icon.png");
    
    glfwSetWindowPos(window,win_xpos,win_ypos);
    glfwSetWindowIcon(window,1,windowIcon);
    
    glfwSetWindowAttrib(window,GLFW_RESIZABLE,GLFW_FALSE);

    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);
    glViewport(0,0,frameWidth,frameHeight);
    
    // V-Sync
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, input_callback);
    
    // SHADER -------------------------------------------------------------------------//
    
    // GL TWEAKS ----------------------------------------------------------------------//
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_STENCIL_TEST);
    //glStencilFunc(GL_EQUAL, 1, 0xFF);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    // CAMERA -------------------------------------------------------------------------//
    
    camera& cam = camera::instance();
    glfwSetScrollCallback(window, scroll_callback);

    cam.set_aspect(frameWidth, frameHeight);
    cam.set_position(glm::vec3(0.0f,0.0f,3.0f));
    
    // GL MATHEMATICS -----------------------------------------------------------------//
    
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection = cam.getPerspective();

    glm::vec3 cubePos(0.0f,0.0f, 0.0f);
    model = glm::translate(model, cubePos);

    glm::mat3 normalModel(1.0f);

    // LIGHTING -----------------------------------------------------------------------//

    // LINES --------------------------------------------------------------------------//

    line line1;
    line1.initLines(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f));

    glm::vec3 top_normal(0.0f,3.0f,0.0f);
    line line2;
    line2.initLines(cubePos, top_normal);

    // LOOP CONTROLLERS ---------------------------------------------------------------//

    bool isRunning = true;

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

    // MODELS -------------------------------------------------------------------------//

    //model3D cup("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/models/backpack/survival_guitar_backpack.obj");

    scene1 scene1;
    scene1.init();

    frame_buffer fb1(frameWidth, frameHeight);
    
    shader sf(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/frame_buffer.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/frame_buffer.frag"
    );

    const unsigned int& frameShader = sf.get_program();

    glClearColor(0.065f,0.0f,0.1f,1.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // OPENGL LOOP --------------------------------------------------------------------//

    while(!glfwWindowShouldClose(window) && isRunning){

        // Time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Camera
        cam.input_handler(window,deltaTime);
        cam.mouse_handler(window);
        cam.scroll_handler(scrollOffset);
        
        cam.look_at();
        view = cam.getView();
        projection = cam.getPerspective();
        
        // Updates //
        if(!isPaused){
            lights::instance().flashlight.isVisible = useFlashLight;
        }

        scene1.update(deltaTime);

        // Cube-Rotation
        /*if(rotateCube){
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0,1.0,1.0));
            normalModel = glm::transpose(glm::inverse(glm::mat3(model)));
        }*/
        
        // Rendering //
        glBindFramebuffer(GL_FRAMEBUFFER, fb1.get_FBO());

        glClearColor(0.065f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        scene1.render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(frameShader);
        glBindVertexArray(fb1.get_VAO());
        glDisable(GL_DEPTH_TEST);

        glBindTexture(GL_TEXTURE_2D, fb1.get_TEX());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

        // Events //
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // TERMINATION --------------------------------------------------------------------//

    //glDeleteProgram(frameShader);
    
    stbi_image_free(windowIcon->pixels);
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

//-----------------------------------------------------------------------------------------------//

GLFWimage* load_image(const char* path) {
    
    GLFWimage* img = new GLFWimage();
    img->pixels = stbi_load(path,&img->width,&img->height,nullptr,4);
    return img;
}

void input_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if(glfwGetKey(window, GLFW_KEY_ESCAPE)){
        glfwSetWindowShouldClose(window,GLFW_TRUE);
    }

    isPaused = (glfwGetKey(window, GLFW_KEY_F))? !isPaused : isPaused;

    if(glfwGetKey(window, GLFW_KEY_E)){

        if(!mouseInCamera){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        mouseInCamera = !mouseInCamera;
    }

    // Hold
    rotateCube = glfwGetKey(window, GLFW_KEY_R);

    // Toggle
    showLines = (glfwGetKey(window, GLFW_KEY_L))? !showLines : showLines;
    useFlashLight = (glfwGetKey(window, GLFW_KEY_T))? !useFlashLight : useFlashLight;
}

void scroll_callback(GLFWwindow* window, double offset_x, double offset_y) {
    scrollOffset = (float)offset_y;
}

void track_mouse(GLFWwindow* window){

    static bool mouse_click_hold = false;

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {

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

    if(!mouse_click_hold) {
        prev_x = pos_x;
        prev_y = pos_y;
        mouse_click_hold = true;
    }

    cursor_dx = pos_x - prev_x;
    cursor_dy = pos_y - prev_y;

    prev_x = pos_x;
    prev_y = pos_y;
}