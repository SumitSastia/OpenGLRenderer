#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(){
    
    vector <int> v1 = {1,2,3};
    vector <int> v2 = {4,5,6};

    return 0;
}

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