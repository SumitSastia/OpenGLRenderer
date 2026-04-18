#include <scenes.h>
#include <camera.h>
#include <lights.h>
#include <iostream>

namespace scenes {

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

        pointShadow_frame_shader = createShader2(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.geom",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.frag"
        );

        gbufferShader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer.frag"
        );

        gbufferInstanced = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/instance/texture.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer.frag"
        );

        gbufferPlanes = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/gbuffer.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/gbuffer.frag"
        );

        shaderPBR = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer_pbr.frag"
        );

        // World Coordinates
        objectModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

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
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.5f, -3.0f));
        lightModel = glm::scale(lightModel, glm::vec3(0.5f));

        myLight = new lights::lightSource(lightModel);
        myLight->setLightColor(colors::yellow);
        myLight->setPosition(lightModel[3]);

        lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 1.5f, 3.0f));
        lightModel = glm::scale(lightModel, glm::vec3(0.5f));

        light2 = new lights::lightSource(lightModel);
        light2->setLightColor(colors::pink);
        light2->setPosition(lightModel[3]);

        lights.push_back(myLight);
        lights.push_back(light2);

        lights_count = 2;

        float near = 1.0f;
        float far = 25.0f;

        shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, near, far);

        // Models
        cube1 = new models::model3D("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/models/test_cube/sphere.obj");

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
        skyboxIntensity = skybox_isVisible ? 1.0f : 0.25f;

        // Multiple Cubes
        totalCubes = 10;
        cubePositions = new glm::vec3[10]
        {
            glm::vec3(5.0f,  0.0f,  0.0f),
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
        cc1 = new coloredCube(colors::red);

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

        // FrameBuffers
        light_frames.push_back(new frameBuffers::pointShadow_frame());
        light_frames.push_back(new frameBuffers::pointShadow_frame());

        // ShadowRendering
        this->render_pointShadow();
    }

    void scene1::input_handler(GLFWwindow* window) {

        static bool toggle_KEY_G = false;
        static bool toggle_KEY_M = false;

        if (glfwGetKey(window, GLFW_KEY_G)) {

            skybox_isVisible = (toggle_KEY_G) ? skybox_isVisible : !skybox_isVisible;

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

            const float rotation_speed = 2.0f;

            glm::mat4 lightModel = lights[0]->getModel();

            lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_speed), glm::vec3(0.0f, 1.0f, 0.0f)) * lightModel;
            lights[0]->update(lightModel);

            lightModel = lights[1]->getModel();

            lightModel = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_speed), glm::vec3(0.0f, 1.0f, 0.0f)) * lightModel;
            lights[1]->update(lightModel);

            this->render_pointShadow();

            /*const glm::vec3 newLightPos = glm::vec3(lightModel[3]);

            glm::mat4 lightView = glm::lookAt(
                newLightPos,
                glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)
            );

            lightSpace = lightProjection * lightView;*/
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

    void scene1::render_pointShadow() const {

        const unsigned int& shader = pointShadow_frame_shader;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDisable(GL_CULL_FACE);

        for (unsigned int i = 0; i < lights_count; i++) {

            const unsigned int currentFrame = light_frames[i]->fbo;
            const glm::vec3 lightPos = lights[i]->getPosition();

            const std::vector <glm::mat4> shadowMatrices = {

            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f,0.0f), glm::vec3(0.0f,0.0f, 1.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,0.0f, 1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,-1.0f,0.0f))
            };

            glBindFramebuffer(GL_FRAMEBUFFER, currentFrame);
            glViewport(0, 0, shadowSize, shadowSize);
            glClear(GL_DEPTH_BUFFER_BIT);

            glUseProgram(shader);

            for (unsigned int i = 0; i < 6; i++) {
                setMat4(shader, ("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowMatrices[i]);
            }

            setVec3(shader, "lightPos", lightPos);
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

            setVec3(pointShadowInstanced, "lightPos", lightPos);
            setFloat(pointShadowInstanced, "far_plane", 25.0f);

            shapes::instance().cubeInstanced.drawShadow(totalCubes);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void scene1::render() const {

        unsigned int shader = 0;

        const glm::mat4& projection = camera::instance().getPerspective();
        const glm::mat4& view = camera::instance().getView();

        lights::lights::instance().update();


        // Object
        shader = textureShader;
        glUseProgram(shader);
        // setMat4(shader, "lightSpace", lightSpace);
        // setInt(shader, "depthMap", 3);
        // 
        // setFloat(shader, "skyboxIntensity", skyboxIntensity);
        // setPointLight(shader, "p1", myLight->getLight());
        // shapes::instance().cube.draw(shader, objectModel);

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
        // shader = pointShadowPlanes;
        // glUseProgram(shader);
        // setMat4(shader, "lightSpace", lightSpace);
        // setInt(shader, "depthMap", 3);
        // setFloat(shader, "skyboxIntensity", 0.8 * skyboxIntensity);
        // setMaterial(shader, "m1", materials::concrete);
        // 
        // setPointLight(shader, "p1", myLight->getLight());
        // floor.draw(shader, floorModel);

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

        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);

        for (unsigned int i = 0; i < lights_count; i++) {

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, light_frames[i]->texture_id);
        }

        unsigned int shader = 0;

        const glm::mat4& projection = camera::instance().getPerspective();
        const glm::mat4& view = camera::instance().getView();

        lights::lights::instance().update();

        // Light-Source
        for (unsigned int i = 0; i < lights_count; i++) {
            lights[i]->draw(lightShader);
        }

        // Object
        shader = textureShader;
        glUseProgram(shader);
        setInt(shader, "lights_count", lights_count);

        for (unsigned int i = 0; i < lights_count; i++) {

            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
            setVec3(shader, ("lightPos[" + std::to_string(i) + "]").c_str(), lights[i]->getPosition());
            setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
        }

        setFloat(shader, "far_plane", 25.0f);

        setFloat(shader, "skyboxIntensity", skyboxIntensity);
        shapes::instance().cube.draw(shader, objectModel);

        // Multiple Cubes
        shader = instanceShader;
        glUseProgram(instanceShader);

        setInt(shader, "lights_count", lights_count);

        for (unsigned int i = 0; i < lights_count; i++) {

            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
            setVec3(shader, ("lightPos[" + std::to_string(i) + "]").c_str(), lights[i]->getPosition());
            setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
        }

        setFloat(shader, "far_plane", 25.0f);

        setFloat(instanceShader, "skyboxIntensity", skyboxIntensity);
        shapes::instance().cubeInstanced.draw(instanceShader, totalCubes);

        // Floor
        shader = pointShadowPlanes;
        glUseProgram(shader);

        setInt(shader, "lights_count", lights_count);

        for (unsigned int i = 0; i < lights_count; i++) {

            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
            setVec3(shader, ("lightPos[" + std::to_string(i) + "]").c_str(), lights[i]->getPosition());
            setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
        }

        setFloat(shader, "far_plane", 25.0f);

        setFloat(shader, "skyboxIntensity", 0.8 * skyboxIntensity);
        setMaterial(shader, "m1", materials::concrete);

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

    void scene1::render_lights() const {

        const glm::mat4& projection = camera::instance().getPerspective();
        const glm::mat4& view = camera::instance().getView();

        // Light-Source
        for (unsigned int i = 0; i < lights_count; i++) {
            lights[i]->draw(lightShader);
        }

        // Skybox
        if (skybox_isVisible) {

            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_FALSE);
            glDisable(GL_CULL_FACE);
            glUseProgram(cubemapShader);

            setMat4(cubemapShader, "projection", camera::instance().getPerspective());
            setMat4(cubemapShader, "view", glm::mat4(glm::mat3(camera::instance().getView())));
            setInt(cubemapShader, "cubeMap", 2);

            glBindVertexArray(skybox->get_VAO());
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->get_ID());
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glEnable(GL_CULL_FACE);
        }
    }

    void scene1::renderPBR() const {

        const unsigned int& shader = shaderPBR;

        glUseProgram(shader);

        setMat4(shader, "projection", camera::instance().getPerspective());
        setMat4(shader, "view", camera::instance().getView());
        setMat4(shader, "model", cubeModel);
        setMat3(shader, "normalModel", glm::transpose(glm::inverse(glm::mat3(cubeModel))));

        cube1->bindTextures(shader);

        setInt(shader, "light_count", 1);
        setFloat(shader, "metallic", 0.1F);
        setFloat(shader, "roughness", 0.2F);

        setVec3(shader, "camPos", camera::instance().getPos());
        setVec3(shader, "lightPosition[0]", lights[0]->getPosition());
        setVec3(shader, "lightColor[0]", lights[0]->getLightColor());

        cube1->draw_gbuffer(shader, cubeModel);
    }

    void scene1::render_g_buffer() const {

        // Object
        // shapes::instance().cube.draw_gbuffer(gbufferShader, objectModel);

        // Multiple Cubes
        shapes::instance().cubeInstanced.draw_gbuffer(gbufferInstanced, totalCubes);

        // Floor
        floor.draw_gbuffer(gbufferPlanes, floorModel);

        // Model
        // cube1->draw_gbuffer(gbufferShader, cubeModel);
        // this->renderPBR();

    }

    void scene1::render_final(const frameBuffers::g_buffer* _g_buffer) const {

        const unsigned int shader = _g_buffer->get_defaultShader();

        glUseProgram(shader);
        setInt(shader, "lights_count", lights_count);
        setFloat(shader, "far_plane", 25.0f);
        setFloat(shader, "skyboxIntensity", skyboxIntensity);

        for (unsigned int i = 0; i < lights_count; i++) {

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, light_frames[i]->texture_id);

            setVec3(shader, ("lights[" + std::to_string(i) + "].position").c_str(), lights[i]->getPosition());
            setVec3(shader, ("lights[" + std::to_string(i) + "].color").c_str(), lights[i]->getLightColor());
            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
        }

        setVec3(shader, "viewPos", camera::instance().getPos());
        lights::lights::instance().update();
        lights::setSpotLight(shader, "torch", lights::lights::instance().flashlight);

        _g_buffer->renderDefault();  
    }

    void scene1::renderSSAO(const frameBuffers::g_buffer* _g_buffer) const {

        const unsigned int shader = _g_buffer->ssaoShader;

        glUseProgram(shader);
        setInt(shader, "gPosition", 0);
        setInt(shader, "gNormal", 1);
        setInt(shader, "gTexture", 2);

        _g_buffer->renderSSAO();
    }

    void scene1::render_transparent() const {

        glEnable(GL_BLEND);

        unsigned int shader = 0;

        const glm::mat4& projection = camera::instance().getPerspective();
        const glm::mat4& view = camera::instance().getView();

        // Model
        /*shader = modelShader;
        glUseProgram(shader);
        setPointLight(shader, "p1", myLight->getLight());
        cube1->draw(shader, cubeModel);*/

        // Cube
        shader = textureShader;
        glUseProgram(shader);
        setInt(shader, "lights_count", lights_count);

        for (unsigned int i = 0; i < lights_count; i++) {

            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
            setVec3(shader, ("lightPos[" + std::to_string(i) + "]").c_str(), lights[i]->getPosition());
            setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
        }

        setFloat(shader, "far_plane", 25.0f);
        setVec3(shader, "viewPos", camera::instance().getPos());

        setFloat(shader, "metallic", 0.1f);
        setFloat(shader, "roughness", 0.3f);

        setFloat(shader, "skyboxIntensity", skyboxIntensity);
        shapes::instance().cube.draw(shader, objectModel);

        // Plane
        shader = pointShadowPlanes;
        glUseProgram(shader);

        setInt(shader, "lights_count", lights_count);

        for (unsigned int i = 0; i < lights_count; i++) {

            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
            setVec3(shader, ("lightPos[" + std::to_string(i) + "]").c_str(), lights[i]->getPosition());
            setPointLight(shader, ("plights[" + std::to_string(i) + "]").c_str(), lights[i]->getLight());
        }

        setFloat(shader, "far_plane", 25.0f);

        setFloat(shader, "skyboxIntensity", 0.8 * skyboxIntensity);
        setMaterial(shader, "m1", materials::concrete);

        shapes::instance().square.draw(shader, windowModel);

        glDisable(GL_BLEND);
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

        for (auto& ptr : light_frames) delete ptr;

        delete[] cubePositions;
    }

///////////////////////////////////////////////////////////////////////////////////////////////

    void scene2::init() {

        gbufferPlanes = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/gbuffer.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/planes/gbuffer.frag"
        );

        objectModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

        floorModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
        floorModel = glm::scale(floorModel, glm::vec3(25.0f));

        wallModel = floorModel;
        wallModel2 = floorModel;

        wallModel = glm::translate(wallModel, glm::vec3(0.0f, 0.0f, -0.5f));
        wallModel2 = glm::translate(wallModel2, glm::vec3(-0.5f, 0.0f, 0.0f));
        wallModel2 = glm::rotate(wallModel2, glm::radians(90.0f),glm::vec3(0.0f, 1.0f, 0.0f));

        floorModel = glm::rotate(floorModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // Floor
        floor = shapes::instance().square;
        floor.loadTexture("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/brickwall.jpg");

        cubeModel0 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        cubeModel0 = glm::scale(cubeModel0, glm::vec3(0.75f));

        cubeModel1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.80f, 0.0f));
        cubeModel1 = glm::scale(cubeModel1, glm::vec3(0.75f));

        gbuffer3D = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/gbuffer.frag"
        );

        light_shader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.frag"
        );

        lights_count = 1;

        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.5f, -1.0f));
        lightModel = glm::scale(lightModel, glm::vec3(0.5f));

        lights::lightSource* myLight = new lights::lightSource(lightModel);
        myLight->setLightColor(colors::yellow);
        myLight->setPosition(lightModel[3]);

        lights.push_back(myLight);

        light_frames.push_back(new frameBuffers::pointShadow_frame());

        float near = 1.0f;
        float far = 25.0f;

        shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, near, far);

        this->render_pointShadow();
    }

    void scene2::render_pointShadow() const {

        const unsigned int& shader = pointShadowFrame_shader;

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDisable(GL_CULL_FACE);

            for (unsigned int i = 0; i < lights_count; i++) {

                const unsigned int currentFrame = light_frames[i]->fbo;
                const glm::vec3 lightPos = lights[i]->getPosition();

                const std::vector <glm::mat4> shadowMatrices = {

                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f,0.0f), glm::vec3(0.0f,0.0f, 1.0f)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,0.0f, 1.0f), glm::vec3(0.0f,-1.0f,0.0f)),
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,-1.0f,0.0f))
                };

                glBindFramebuffer(GL_FRAMEBUFFER, currentFrame);
                glViewport(0, 0, shadowSize, shadowSize);
                glClear(GL_DEPTH_BUFFER_BIT);

                glUseProgram(shader);

                for (unsigned int i = 0; i < 6; i++) {
                    setMat4(shader, ("shadowMatrices[" + std::to_string(i) + "]").c_str(), shadowMatrices[i]);
                }

                setVec3(shader, "lightPos", lightPos);
                setFloat(shader, "far_plane", 25.0f);

                // Object
                setMat4(shader, "model", cubeModel0);
                shapes::instance().cube.drawShadow();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

    void scene2::render() {

        // Floor
        // floor.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        // floor.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        // floor.draw_gbuffer(gbufferPlanes, floorModel);

        // floor.normal = glm::vec3(0.0f, 0.0f, 1.0f);
        // floor.draw_gbuffer(gbufferPlanes, wallModel);

        // floor.normal = glm::vec3(1.0f, 0.0f, 0.0f);
        // floor.draw_gbuffer(gbufferPlanes, wallModel2);

        floor.draw_gbuffer(gbufferPlanes, wallModel);

        // Object
        shapes::instance().cube.draw_gbuffer(gbuffer3D, cubeModel0);
        shapes::instance().cube.draw_gbuffer(gbuffer3D, cubeModel1);
    }

    void scene2::render_final(const frameBuffers::g_buffer* _g_buffer) const {
        
        // _g_buffer->renderSSAO();
        // _g_buffer->render();

        const unsigned int shader = _g_buffer->get_defaultShader();

        glUseProgram(shader);

        setInt(shader, "lights_count", lights_count);
        setFloat(shader, "far_plane", 25.0f);
        setFloat(shader, "skyboxIntensity", 1.0);

        for (unsigned int i = 0; i < lights_count; i++) {
        
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, light_frames[i]->texture_id);
        
            setVec3(shader, ("lights[" + std::to_string(i) + "].position").c_str(), lights[i]->getPosition());
            setVec3(shader, ("lights[" + std::to_string(i) + "].color").c_str(), lights[i]->getLightColor());
            setInt(shader, ("depthCubeMap[" + std::to_string(i) + "]").c_str(), i);
        }

        setVec3(shader, "viewPos", camera::instance().getPos());
        lights::lights::instance().update();
        lights::setSpotLight(shader, "torch", lights::lights::instance().flashlight);

        _g_buffer->renderDefault();
  
        // Light-Source
        for (unsigned int i = 0; i < 1; i++) {
            lights[i]->draw(light_shader);
        }
    }
}
