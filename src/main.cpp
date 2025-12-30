#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <shader.hpp>
#include <camera.hpp>
#include <lights.hpp>
#include <mesh.hpp>

#define WIN_W 1120
#define WIN_H 700

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
    
    int win_xpos = videoMode->width/2 - WIN_W/2;
    int win_ypos = videoMode->height/2 - WIN_H/2;
    
    GLFWimage* windowIcon = load_image("window_icon.png");
    
    glfwSetWindowPos(window,win_xpos,win_ypos);
    glfwSetWindowIcon(window,1,windowIcon);
    
    glfwSetWindowAttrib(window,GLFW_RESIZABLE,GLFW_FALSE);
    
    glfwGetFramebufferSize(window, &frameWidth, &frameHeight);

    glViewport(0,0,frameWidth,frameHeight);
    glClearColor(0.065f,0.0f,0.1f,0.5f);
    
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, input_callback);
    
    // BUFFER -------------------------------------------------------------------------//

    float vertices[] = {

        // Front
        -0.5, 0.5, 0.5,  0.0,0.0,  0.0,0.0,1.0,
         0.5, 0.5, 0.5,  1.0,0.0,  0.0,0.0,1.0,
        -0.5,-0.5, 0.5,  0.0,1.0,  0.0,0.0,1.0,
         0.5,-0.5, 0.5,  1.0,1.0,  0.0,0.0,1.0,
         
        // Back
         0.5, 0.5,-0.5,  0.0,0.0,  0.0,0.0,-1.0,
        -0.5, 0.5,-0.5,  1.0,0.0,  0.0,0.0,-1.0,
         0.5,-0.5,-0.5,  0.0,1.0,  0.0,0.0,-1.0,
        -0.5,-0.5,-0.5,  1.0,1.0,  0.0,0.0,-1.0,

        // Left
        -0.5, 0.5,-0.5,  0.0,0.0,  -1.0,0.0,0.0,
        -0.5, 0.5, 0.5,  1.0,0.0,  -1.0,0.0,0.0,
        -0.5,-0.5,-0.5,  0.0,1.0,  -1.0,0.0,0.0,
        -0.5,-0.5, 0.5,  1.0,1.0,  -1.0,0.0,0.0,

        // Right
         0.5, 0.5, 0.5,  0.0,0.0,  1.0,0.0,0.0,
         0.5, 0.5,-0.5,  1.0,0.0,  1.0,0.0,0.0,
         0.5,-0.5, 0.5,  0.0,1.0,  1.0,0.0,0.0,
         0.5,-0.5,-0.5,  1.0,1.0,  1.0,0.0,0.0,

        // Top
        -0.5, 0.5,-0.5,  0.0,0.0,  0.0,1.0,0.0,
         0.5, 0.5,-0.5,  1.0,0.0,  0.0,1.0,0.0,
        -0.5, 0.5, 0.5,  0.0,1.0,  0.0,1.0,0.0,
         0.5, 0.5, 0.5,  1.0,1.0,  0.0,1.0,0.0,

        // Bottom
         0.5,-0.5,-0.5,  0.0,0.0,  0.0,-1.0,0.0,
        -0.5,-0.5,-0.5,  1.0,0.0,  0.0,-1.0,0.0,
         0.5,-0.5, 0.5,  0.0,1.0,  0.0,-1.0,0.0,
        -0.5,-0.5, 0.5,  1.0,1.0,  0.0,-1.0,0.0,
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
    
    buffer b1;
    buffer b2; // Light-Source Cube

    b1.init2(vertices, sizeof(vertices), indices, sizeof(indices));
    b2.init(vertices2, sizeof(vertices2), indices2, sizeof(indices2));
    
    unsigned int VAO = b1.get_VAO();
    unsigned int VAO2 = b2.get_VAO();

    unsigned int lineShader = s1->get_program();
    unsigned int lightShader = s2->get_program();
    unsigned int textureShader = s3->get_program();
    unsigned int modelShader = s4->get_program();
    
    // Textures -----------------------------------------------------------------------//
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    texture wood_texture;
    texture metal_frame;
    texture emission;

    wood_texture.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\wood_box.png");
    metal_frame.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\metal_frame.png");
    emission.load("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\textures\\test.png");

    materials materials;
    lights lights;
    colors colors;
    
    // CAMERA -------------------------------------------------------------------------//
    
    camera cam;
    glfwSetScrollCallback(window, scroll_callback);

    cam.set_aspect(frameWidth, frameHeight);
    
    // GL MATHEMATICS -----------------------------------------------------------------//
    
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection = cam.getPerspective();

    glm::vec3 cubePos(0.0f,0.0f, 0.0f);
    model = glm::translate(model, cubePos);

    glm::mat3 normalModel(1.0f);

    // LIGHTING -----------------------------------------------------------------------//

    glm::vec3 lightColor(colors.yellow);

    glm::vec3 lightPos(3.0f, 1.5f,-3.0f);
    glm::mat4 lightModel(1.0f);
    
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.5f));

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

    model3d bed("C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\assets\\models\\cup\\Cup.obj");
    
    // cout << "didnt even reach here" << endl;

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

            // Line-Position
            line1.updateLines(lightPos, cubePos);
            // line2.updateLines(cubePos, top_normal);

            // Light-Rotation
            float rotationSpeed = 1.0f;
            // lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed), glm::vec3(0.0,1.0,0.0)) * lightModel;

            lightPos = glm::vec3(3.0f, 1.5f,-3.0f);
            lightPos = glm::vec3(lightModel * glm::vec4(lightPos, 1.0f));
        }

        // Cube-Rotation
        if(rotateCube){
            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0,1.0,1.0));
            normalModel = glm::transpose(glm::inverse(glm::mat3(model)));
        }
        
        // Rendering //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Light-Source
        glUseProgram(lightShader);
        glm::mat4 finalMatrix = projection * view * lightModel;
        
        setMat4(lightShader, "finalMatrix", finalMatrix);
        setVec3(lightShader, "lightColor", lightColor);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));

        // Cube - Object1
        // glUseProgram(textureShader);

        // setMat4(textureShader, "projection", projection);
        // setMat4(textureShader, "view", view);
        // setMat4(textureShader, "model", model);

        // setMat3(textureShader, "normalModel", normalModel);
        // setVec3(textureShader, "viewPos", cam.getPos());

        // // Material
        // setVec3(textureShader, "m1.ambient", materials.wood.ambient);
        // setVec3(textureShader, "m1.diffuse", materials.wood.diffuse);
        // setVec3(textureShader, "m1.specular", materials.wood.specular);
        // setFloat(textureShader, "m1.shininess", materials.glass.shininess);

        // // Lights
        // lights.flashlight.position = cam.getPos();
        // lights.flashlight.direction = cam.getTarget();

        // lights.cubelight.position = lightPos;
        // lights.cubelight.color = lightColor;

        // glUniform1i(
        //     glGetUniformLocation(textureShader, "useFlashLight"),
        //     useFlashLight
        // );

        // setDirectionalLight(textureShader, "d1", lights.sunlight);
        // setSpotLight(textureShader, "s1", lights.flashlight);
        // setPointLight(textureShader, "p1", lights.cubelight);

        // glUniform1i(
        //     glGetUniformLocation(textureShader, "texture1"),
        //     0
        // );
        // glUniform1i(
        //     glGetUniformLocation(textureShader, "texture2"),
        //     1
        // );

        // glBindVertexArray(VAO);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, wood_texture.getID());

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, metal_frame.getID());

        // for(unsigned int i=0; i<10; i++){

        //     glm::mat4 model1(1.0f);
        //     model1 = glm::translate(model1, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));
        //     normalModel = glm::transpose(glm::inverse(glm::mat3(model1)));

        //     setMat4(textureShader, "model", model1);
        //     setMat3(textureShader, "normalModel", normalModel);
        //     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        // }

        // Models
        glUseProgram(modelShader);

        setMat4(modelShader, "projection", projection);
        setMat4(modelShader, "view", view);
        setMat4(modelShader, "model", model);

        setMat3(modelShader, "normalModel", normalModel);
        setVec3(modelShader, "viewPos", cam.getPos());

        // Material
        setVec3(modelShader, "m1.ambient", materials.wood.ambient);
        setVec3(modelShader, "m1.diffuse", materials.wood.diffuse);
        setVec3(modelShader, "m1.specular", materials.wood.specular);
        setFloat(modelShader, "m1.shininess", materials.glass.shininess);

        glm::mat4 model1(1.0f);

        model1 = glm::scale(model1, glm::vec3(0.2f));
        normalModel = glm::transpose(glm::inverse(glm::mat3(model1)));

        setMat4(modelShader, "model", model1);
        setMat3(modelShader, "normalModel", normalModel);

        // Lights
        lights.flashlight.position = cam.getPos();
        lights.flashlight.direction = cam.getTarget();

        lights.cubelight.position = lightPos;
        lights.cubelight.color = lightColor;

        glUniform1i(
            glGetUniformLocation(modelShader, "useFlashLight"),
            useFlashLight
        );

        setDirectionalLight(modelShader, "d1", lights.sunlight);
        setSpotLight(modelShader, "s1", lights.flashlight);
        setPointLight(modelShader, "p1", lights.cubelight);

        glUniform1i(
            glGetUniformLocation(modelShader, "texture_diffuse"),
            0
        );
        glUniform1i(
            glGetUniformLocation(modelShader, "texture_specular"),
            1
        );

        bed.draw(modelShader);

        // Lines
        glUseProgram(lineShader);
        glm::mat4 lineModel(1.0f);

        setMat4(lineShader, "projection", projection);
        setMat4(lineShader, "view", view);
        setMat4(lineShader, "lineModel", lineModel);

        if(showLines){
            glBindVertexArray(line1.get_VAO());
            glDrawArrays(GL_LINES, 0, 2);
        }

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

    rotateCube = glfwGetKey(window, GLFW_KEY_R);
    showLines = (glfwGetKey(window, GLFW_KEY_L))? !showLines : showLines;
    useFlashLight = (glfwGetKey(window, GLFW_KEY_T))? !useFlashLight : useFlashLight;
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