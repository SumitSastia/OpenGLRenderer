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
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/texture.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/texture.frag"
    );

    modelShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/model.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/model.frag"
    );

    planeShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes.frag"
    );

    cubemapShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/cubemap.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/cubemap.frag"
    );

    // World Coordinates
    objectModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

    lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.5f, -3.0f));
    lightModel = glm::scale(lightModel, glm::vec3(0.5f));

    cubeModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 7.0f));
    cubeModel = glm::scale(cubeModel, glm::vec3(0.75f));

    // Light-Source
    myLight = new lightSource(camera::instance().getPerspective(), camera::instance().getView(), lightModel);
    myLight->setLightColor(colors::instance().yellow);
    myLight->setPosition(glm::vec3(3.0f, 1.5f, -3.0f));

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
}

void scene1::update(const float& delta_time) {

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();

    // Light-Rotation
    float rotationSpeed = 1.0f;
    lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed), glm::vec3(0.0f,1.0f,0.0f)) * lightModel;

    lights::instance().update();

    myLight->update(projection, view, lightModel);

    shapes::instance().cube.update(projection, view, objectModel);
    shapes::instance().square.update(projection, view, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    cube1->update(projection, view, cubeModel);
}

void scene1::render() const {

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();

    // Light-Source
    myLight->draw(lightShader);

    // Object
    glUseProgram(textureShader);
    setPointLight(textureShader, "p1", myLight->getLight());
    shapes::instance().cube.draw(textureShader);

    // Model
    glUseProgram(modelShader);
    setPointLight(modelShader, "p1", myLight->getLight());
    cube1->draw(modelShader);

    // Skybox
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glUseProgram(cubemapShader);

    setMat4(cubemapShader, "projection", projection);
    setMat4(cubemapShader, "view", glm::mat4(glm::mat3(view)));

    glBindVertexArray(skybox->get_VAO());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->get_ID());
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void scene1::render_transparent() const {

    // Plane
    shapes::instance().square.draw(planeShader);
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
}