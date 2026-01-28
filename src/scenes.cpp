#include <scenes.h>
#include <camera.h>
#include <iostream>

void scene1::init() {

    // Shaders
    lineShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/lines.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/lines.frag"
    );

    lightShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.frag"
    );

    textureShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/textureCube/texture.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/textureCube/texture.frag"
    );

    modelShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/models/model.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/models/model.frag"
    );

    planeShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/planes.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/planes.frag"
    );

    cubemapShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/cubemap.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/cubemap.frag"
    );

    normalShader = createShader2(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/geometry/geometry.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/geometry/geometry.geom",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/geometry/geometry.frag"
    );

    instanceShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/instance/texture.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/instance/texture.frag"
    );

    instanceShadowShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/shadow/instanceShadow.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/shadow/shadow.frag"
    );

    shadowShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/shadow/shadow.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/shadow/shadow.frag"
    );

    pointShadowPlanes = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/plane_with_shadow.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/plane_with_shadow.frag"
    );

    pointShadowInstanced = createShader2(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/instanceShadow.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.geom",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.frag"
    );

    // World Coordinates
    objectModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

    lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.5f, -3.0f));
    lightModel = glm::scale(lightModel, glm::vec3(0.5f));

    lightModel2 = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 1.5f, 3.0f));
    lightModel2 = glm::scale(lightModel2, glm::vec3(0.5f));

    cubeModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 7.0f));
    cubeModel = glm::scale(cubeModel, glm::vec3(0.75f));

    windowModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
    floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    floorModel = glm::scale(floorModel, glm::vec3(25.0f));

    worldModels.push_back(&objectModel);
    worldModels.push_back(&cubeModel);
    worldModels.push_back(&windowModel);

    // Light-Source
    lights_count = 2;

    myLight = new lightSource(camera::instance().getPerspective(), camera::instance().getView(), lightModel);
    myLight->setLightColor(colors::instance().yellow);
    myLight->setPosition(glm::vec3(3.0f, 1.5f, -3.0f));

    light2 = new lightSource(camera::instance().getPerspective(), camera::instance().getView(), lightModel2);
    light2->setLightColor(colors::instance().pink);
    light2->setPosition(glm::vec3(-3.0f, 1.5f, 3.0f));

    lights.push_back(myLight);
    lights.push_back(light2);

    float near = 1.0f;
    float far = 25.0f;

    shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, near, far);

    // Models
    cube1 = new model3D("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/models/test_cube/cube.obj");

    // Skybox
    std::vector <std::string> cubemapFaces = {
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/skybox/Daylight_Box_Right.bmp",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/skybox/Daylight_Box_Left.bmp",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/skybox/Daylight_Box_Top.bmp",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/skybox/Daylight_Box_Bottom.bmp",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/skybox/Daylight_Box_Front.bmp",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/skybox/Daylight_Box_Back.bmp"
    };

    skybox = new cubeMap(cubemapFaces);
    skybox_isVisible = false;
    skyboxIntensity = skybox_isVisible? 1.0f : 0.25f;

    // Multiple Cubes
    totalCubes = 10;
    cubePositions = new glm::vec3[10]
    {
        glm::vec3( 5.0f,  0.0f,  0.0f),
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

    glUseProgram(instanceShader);
    setMat4(instanceShader, "projection", camera::instance().getPerspective());
    setMat4(instanceShader, "view", camera::instance().getView());

    glm::mat4 objModels[10];
    glm::mat3 objNormals[10];

    for (unsigned int i = 0; i < totalCubes; i++) {

        glm::mat4 objModel = glm::translate(glm::mat4(1.0f), cubePositions[i]);

        objModels[i] = glm::rotate(objModel, i * 15.0f, glm::vec3(1.0f, 2.0f, 3.0f));
        objNormals[i] = glm::transpose(glm::inverse(glm::mat3(objModels[i])));
    }

    glGenBuffers(1, &instanceModelVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
    glBufferData(GL_ARRAY_BUFFER, totalCubes * sizeof(glm::mat4), objModels, GL_STATIC_DRAW);

    glGenBuffers(1, &instanceNormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceNormalVBO);
    glBufferData(GL_ARRAY_BUFFER, totalCubes * sizeof(glm::mat3), objNormals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, instanceModelVBO);
    glBindVertexArray(shapes::instance().cubeInstanced.VAO);

    std::size_t vec3Size = sizeof(glm::vec3);
    std::size_t vec4Size = sizeof(glm::vec4);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
    glVertexAttribDivisor(4, 1);
    
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glVertexAttribDivisor(5, 1);
    
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, instanceNormalVBO);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * vec3Size, (void*)0);
    glVertexAttribDivisor(7, 1);

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 3 * vec3Size, (void*)(1 * vec3Size));
    glVertexAttribDivisor(8, 1);

    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 3 * vec3Size, (void*)(2 * vec3Size));
    glVertexAttribDivisor(9, 1);

    glBindVertexArray(0);

    // ColoredCube
    cc1 = new coloredCube(colors::instance().red);

    // Floor
    floor = shapes::instance().square;
    floor.loadTexture("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/brickwall.jpg");

    // Light-Space
    float near_plane = 1.0f, far_plane = 20.0f, size = 5.0f;
    lightProjection = glm::ortho(-size, size, -size, size, near_plane, far_plane);

    glm::mat4 lightView = glm::lookAt(
        myLight->getPosition(),
        glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
    );

    lightSpace = lightProjection * lightView;
}

void scene1::input_handler(GLFWwindow* window) {

    static bool toggle_KEY_G = false;
    static bool toggle_KEY_M = false;

    if (glfwGetKey(window, GLFW_KEY_G)) {

        skybox_isVisible = (toggle_KEY_G)? skybox_isVisible : !skybox_isVisible;

        if (skybox_isVisible) {
            skyboxIntensity = 1.0f;
        }
        else {
            skyboxIntensity = 0.25f;
        }

        toggle_KEY_G = true;
    }
    else { toggle_KEY_G = false; }

    if (glfwGetKey(window, GLFW_KEY_M)) {

        if (!toggle_KEY_M) {
            modelCounter = (modelCounter + 1) % worldModels.size();
        }
        toggle_KEY_M = true;
    }
    else { toggle_KEY_M = false; }

    // Model Position Updater
    if (glfwGetKey(window, GLFW_KEY_KP_8)) {
        *(worldModels[modelCounter]) = glm::translate(*(worldModels[modelCounter]), glm::vec3(0.0f, 0.01f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2)) {
        *(worldModels[modelCounter]) = glm::translate(*(worldModels[modelCounter]), glm::vec3(0.0f, -0.01f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4)) {
        *(worldModels[modelCounter]) = glm::translate(*(worldModels[modelCounter]), glm::vec3(-0.01f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6)) {
        *(worldModels[modelCounter]) = glm::translate(*(worldModels[modelCounter]), glm::vec3(0.01f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_KP_7)) {
        *(worldModels[modelCounter]) = glm::translate(*(worldModels[modelCounter]), glm::vec3(0.0f, 0.0f, -0.01f));
    }
    if (glfwGetKey(window, GLFW_KEY_KP_9)) {
        *(worldModels[modelCounter]) = glm::translate(*(worldModels[modelCounter]), glm::vec3(0.0f, 0.0f, 0.01f));
    }

    // Light-Rotater
    if (glfwGetKey(window, GLFW_KEY_KP_5)) {

        lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * lightModel;
        lightModel2 = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * lightModel2;

        const glm::vec3 newLightPos = glm::vec3(lightModel[3]);

        glm::mat4 lightView = glm::lookAt(
            newLightPos,
            glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
        );

        lightSpace = lightProjection * lightView;
    }
}

void scene1::update(const float& delta_time) {

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();
}

void scene1::render_shadow(const unsigned int& shader) const {

    glUseProgram(shader);
    setMat4(shader, "lightSpace", lightSpace);

    // Object
    setMat4(shader, "model", objectModel);
    shapes::instance().cube.drawShadow();

    // Model
    setMat4(shader, "model", cubeModel);
    cube1->drawShadow();

    // Window
    setMat4(shader, "model", windowModel);
    shapes::instance().square.drawShadow();

    // Multiple Cubes - !! Uses Separate Shader
    /*glUseProgram(instanceShadowShader);
    setMat4(instanceShadowShader, "lightSpace", lightSpace);

    shapes::instance().cubeInstanced.drawShadow(totalCubes);*/
}

void scene1::render_pointShadow(const unsigned int& shader) const {

    const glm::vec3 lightPos = myLight->getPosition();

    const std::vector <glm::mat4> shadowMatrices = {

        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f,0.0f), glm::vec3(0.0f,0.0f, 1.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,0.0f, 1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
        shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,-1.0f,0.0f))
    };

    glUseProgram(shader);

    for (unsigned int i = 0; i < 6; i++) {
        setMat4(shader, ("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowMatrices[i]);
    }

    setVec3(shader, "lightPos", myLight->getPosition());
    setFloat(shader, "far_plane", 25.0f);

    // Object
    setMat4(shader, "model", objectModel);
    shapes::instance().cube.drawShadow();

    // Model
    setMat4(shader, "model", cubeModel);
    cube1->drawShadow();

    // Window
    setMat4(shader, "model", windowModel);
    shapes::instance().square.drawShadow();

    // Multiple Cubes - !! Uses Separate Shader
    glUseProgram(pointShadowInstanced);

    for (unsigned int i = 0; i < 6; i++) {
        setMat4(pointShadowInstanced, ("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowMatrices[i]);
    }

    setVec3(pointShadowInstanced, "lightPos", myLight->getPosition());
    setFloat(pointShadowInstanced, "far_plane", 25.0f);

    shapes::instance().cubeInstanced.drawShadow(totalCubes);
}

void scene1::render() const {

    unsigned int shader = 0;

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();
    
    lights::instance().update();

    // Light-Source
    myLight->update(projection, view, lightModel);
    myLight->draw(lightShader);

    // Object
    shader = textureShader;
    glUseProgram(shader);
    setMat4(shader, "lightSpace", lightSpace);
    setInt(shader, "depthMap", 3);

    setFloat(shader, "skyboxIntensity", skyboxIntensity);
    setPointLight(shader, "p1", myLight->getLight());
    shapes::instance().cube.draw(shader, objectModel);

    // Multiple Cubes
    /*glUseProgram(instanceShader);
    setMat4(instanceShader, "lightSpace", lightSpace);
    setInt(instanceShader, "depthMap", 3);

    setFloat(instanceShader, "skyboxIntensity", skyboxIntensity);
    setPointLight(instanceShader, "p1", myLight->getLight());
    shapes::instance().cubeInstanced.draw(instanceShader, totalCubes);*/

    // Normal-Visualizer
    // shapes::instance().cube.draw(normalShader);

    // ColoredCube
    // cc1->render();

    // Floor
    shader = pointShadowPlanes;
    glUseProgram(shader);
    setMat4(shader, "lightSpace", lightSpace);
    setInt(shader, "depthMap", 3);
    setFloat(shader, "skyboxIntensity", 0.8 * skyboxIntensity);
    setMaterial(shader, "m1", materials::instance().concrete);

    setPointLight(shader, "p1", myLight->getLight());
    floor.draw(shader, floorModel);

    // Skybox
    if (skybox_isVisible) {

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glUseProgram(cubemapShader);

        setMat4(cubemapShader, "projection", projection);
        setMat4(cubemapShader, "view", glm::mat4(glm::mat3(view)));
        setInt(cubemapShader, "cubeMap", 2);

        glBindVertexArray(skybox->get_VAO());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->get_ID());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
    }

    render_transparent();
}

void scene1::render_with_pointLight() const {

    unsigned int shader = 0;

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();

    lights::instance().update();

    // Light-Source
    myLight->update(projection, view, lightModel);
    myLight->draw(lightShader);

    light2->update(projection, view, lightModel2);
    light2->draw(lightShader);

    // Object
    shader = textureShader;
    glUseProgram(shader);
    setInt(shader, "depthCubeMap", 0);
    setVec3(shader, "lightPos", myLight->getPosition());
    setFloat(shader, "far_plane", 25.0f);
    
    setFloat(shader, "skyboxIntensity", skyboxIntensity);
    setPointLight(shader, "p1", myLight->getLight());
    shapes::instance().cube.draw(shader, objectModel);

    // Multiple Cubes
    shader = instanceShader;
    glUseProgram(instanceShader);

    setInt(shader, "lights_count", lights_count);

    for (unsigned int i = 0; i < lights_count; i++) {
        setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
    }

    setInt(shader, "depthCubeMap", 0);
    setVec3(shader, "lightPos", myLight->getPosition());
    setFloat(shader, "far_plane", 25.0f);

    setFloat(instanceShader, "skyboxIntensity", skyboxIntensity);
    setPointLight(instanceShader, "p1", myLight->getLight());
    shapes::instance().cubeInstanced.draw(instanceShader, totalCubes);

    // Floor
    shader = pointShadowPlanes;
    glUseProgram(shader);

    setInt(shader, "lights_count", lights_count);

    for (unsigned int i = 0; i < lights_count; i++) {
        setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
    }

    setInt(shader, "depthCubeMap", 0);
    setVec3(shader, "lightPos", myLight->getPosition());
    setFloat(shader, "far_plane", 25.0f);

    setFloat(shader, "skyboxIntensity", 0.8 * skyboxIntensity);
    setMaterial(shader, "m1", materials::instance().concrete);

    setPointLight(shader, "p1", myLight->getLight());

    floor.draw(shader, floorModel);

    // Skybox
    if (skybox_isVisible) {

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        glUseProgram(cubemapShader);

        setMat4(cubemapShader, "projection", projection);
        setMat4(cubemapShader, "view", glm::mat4(glm::mat3(view)));
        setInt(cubemapShader, "cubeMap", 2);

        glBindVertexArray(skybox->get_VAO());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->get_ID());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
    }

    render_transparent();
}

void scene1::render_transparent() const {

    unsigned int shader = 0;

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();

    // Model
    shader = modelShader;
    glUseProgram(shader);
    setPointLight(shader, "p1", myLight->getLight());
    cube1->draw(shader, cubeModel);

    // Plane
    shader = pointShadowPlanes;
    glUseProgram(shader);
    setMat4(shader, "lightSpace", lightSpace);
    setInt(shader, "depthMap", 3);
    setPointLight(shader, "p1", myLight->getLight());
    shapes::instance().square.draw(shader, windowModel);
}

void scene1::destroy() const {

    glDeleteProgram(lineShader);
    glDeleteProgram(lightShader);
    glDeleteProgram(textureShader);
    glDeleteProgram(modelShader);
    glDeleteProgram(planeShader);

    delete myLight;
    delete cube1;
    delete skybox;
    delete cc1;

    delete[] cubePositions;
}