#include <scenes.h>
#include <camera.h>
#include <iostream>

void scene1::init() {

    shader s1(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/lines.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/lines.frag"
    );

    shader s2(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.frag"
    );

    shader s3(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/texture.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/texture.frag"
    );

    shader s4(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/model.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/model.frag"
    );

    shader s5(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes.frag"
    );

    // Shaders
    lineShader = s1.get_program();
    lightShader = s2.get_program();
    textureShader = s3.get_program();
    modelShader = s4.get_program();
    planeShader = s5.get_program();

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

    // Plane
    shapes::instance().square.draw(planeShader);

    // Model
    glUseProgram(modelShader);
    setPointLight(modelShader, "p1", myLight->getLight());
    cube1->draw(modelShader);
}

void scene1::destroy() const {

    glDeleteProgram(lineShader);
    glDeleteProgram(lightShader);
    glDeleteProgram(textureShader);
    glDeleteProgram(modelShader);
    glDeleteProgram(planeShader);

    delete myLight;
    delete cube1;
}