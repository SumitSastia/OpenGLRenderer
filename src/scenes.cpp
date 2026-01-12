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

    normalShader = createShader2(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/geometry/geometry.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/geometry/geometry.geom",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/geometry/geometry.frag"
    );

    instanceShader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/instance/texture.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/instance/texture.frag"
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

    // Multiple Cubes
    totalCubes = 10;
    cubePositions = new glm::vec3[10]
    {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glUseProgram(instanceShader);
    setMat4(instanceShader, "projection", camera::instance().getPerspective());
    setMat4(instanceShader, "view", camera::instance().getView());

    for (unsigned int i = 0; i < totalCubes; i++) {

        glm::mat4 objModels = glm::translate(glm::mat4(1.0f), cubePositions[i]);
        objModels = glm::rotate(objModels, i * 15.0f, glm::vec3(1.0f, 2.0f, 3.0f));

        setMat4(instanceShader, ("offsetModels[" + std::to_string(i) + "]").c_str(), objModels);
        setMat3(instanceShader, ("offsetNormals[" + std::to_string(i) + "]").c_str(), glm::transpose(glm::inverse(glm::mat3(objModels))));
    }
}

void scene1::update(const float& delta_time) {

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();

    // Light-Rotation
    float rotationSpeed = 1.0f;
    lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed), glm::vec3(0.0f,1.0f,0.0f)) * lightModel;

}

void scene1::render() const {

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();
    
    lights::instance().update();

    // Light-Source
    myLight->update(projection, view, lightModel);
    myLight->draw(lightShader);

    // Object
    glUseProgram(textureShader);
    setPointLight(textureShader, "p1", myLight->getLight());
    // shapes::instance().cube.update(projection, view, objectModel);
    // shapes::instance().cube.draw(textureShader);

    // Multiple Cubes
    glUseProgram(instanceShader);
    setPointLight(instanceShader, "p1", myLight->getLight());
    shapes::instance().cubeInstanced.draw(instanceShader, totalCubes);

    // Normal-Visualizer
    // shapes::instance().cube.draw(normalShader);

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

    const glm::mat4& projection = camera::instance().getPerspective();
    const glm::mat4& view = camera::instance().getView();

    // Model
    glUseProgram(modelShader);
    setPointLight(modelShader, "p1", myLight->getLight());
    cube1->update(projection, view, cubeModel);
    cube1->draw(modelShader);

    // Plane
    shapes::instance().square.update(projection, view, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
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

    delete[] cubePositions;
}