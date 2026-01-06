#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <shader.hpp>
#include <camera.hpp>
#include <lights.hpp>
#include <models.hpp>
#include <shapes.hpp>

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

    shader* s1 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\lines.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\lines.frag"
    );

    shader* s2 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\light.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\light.frag"
    );

    shader* s3 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\texture.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\texture.frag"
    );

    shader* s4 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\model.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\model.frag"
    );

    shader* s5 = new shader(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\planes.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\planes.frag"
    );

    const unsigned int& lineShader = s1->get_program();
    const unsigned int& lightShader = s2->get_program();
    const unsigned int& textureShader = s3->get_program();
    const unsigned int& modelShader = s4->get_program();
    const unsigned int& planeShader = s5->get_program();
    
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

    glm::vec3 lightColor(colors::instance().yellow);

    glm::vec3 lightPos(3.0f, 1.5f,-3.0f);
    glm::mat4 lightModel(1.0f);

    lights::instance().cubelight.position = lightPos;
    lights::instance().cubelight.color = lightColor;
    
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.5f));

    lightSource myLight(projection, view, lightModel);
    
    myLight.setLightColor(lightColor);
    myLight.setPosition(lightPos);

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
    model3D test_cube("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/models/test_cube/cube.obj");

    glm::mat4 cup_model(1.0f);

    cup_model = glm::translate(cup_model, glm::vec3(0.0f,3.0f,0.0f));
    cup_model = glm::scale(cup_model, glm::vec3(0.5f));

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

            // Line-Position
            line1.updateLines(lightPos, cubePos);
            // line2.updateLines(cubePos, top_normal);

            // Light-Rotation
            float rotationSpeed = 1.0f;
            lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed), glm::vec3(0.0,1.0,0.0)) * lightModel;

            lightPos = glm::vec3(3.0f, 1.5f,-3.0f);
            lightPos = glm::vec3(lightModel * glm::vec4(lightPos, 1.0f));
        }

        // Cube-Rotation
        if(rotateCube){
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0,1.0,1.0));
            normalModel = glm::transpose(glm::inverse(glm::mat3(model)));
        }
        
        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Light-Source
        myLight.update(projection, view, lightModel);
        myLight.draw(lightShader);

        // Lights
        lights::instance().update();

        // Shapes
        glUseProgram(textureShader);

        shapes::instance().cube.update(projection, view, model);
        setPointLight(textureShader, "p1", myLight.getLight());

        shapes::instance().cube.draw(textureShader);

        shapes::instance().square.update(projection, view, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,1.0f)));
        shapes::instance().square.draw(planeShader);

        /*for (unsigned int i = 0; i < 10; i++) {

            glm::mat4 model1(1.0f);
            model1 = glm::translate(model1, cubePositions[i]);

            float angle = 20.0f * i;
            model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            shapes::instance().cube.update(projection, view, model1);
            shapes::instance().cube.draw(textureShader);
        }*/

        // Models
        glUseProgram(modelShader);

        cup_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, 0.0f));
        cup_model = glm::scale(cup_model, glm::vec3(0.5f));

        test_cube.update(projection, view, cup_model);
        setPointLight(modelShader, "p1", myLight.getLight());

        test_cube.draw(modelShader);

        /*for (unsigned int i = 0; i < 10; i++) {

            glm::mat4 model1(1.0f);
            model1 = glm::translate(model1, cubePositions[i]);

            float angle = 20.0f * i;
            model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            test_cube.update(projection, view, model1);
            test_cube.draw(modelShader);
        }*/

        // Lines
        glUseProgram(lineShader);
        glm::mat4 lineModel(1.0f);

        setMat4(lineShader, "finalMatrix", projection * view * lineModel);

        if(showLines){
            glBindVertexArray(line1.get_VAO());
            glDrawArrays(GL_LINES, 0, 2);
        }

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