#include <lights.hpp>
#include <shader.hpp>

lights& lights::instance() {
    static lights instance;
    return instance;
}

colors& colors::instance() {
    static colors instance;
    return instance;
}

lights::lights() {

    cubelight.position = glm::vec3(3.0f, 1.5f,-3.0f);
    cubelight.color = glm::vec3(1.0f,1.0f,1.0f);

    cubelight.constant = 1.0f;
    cubelight.linear = 0.001f;
    cubelight.quadratic = 0.016f;

    sunlight.color = glm::vec3(0.99f,0.82f,0.66f);
    sunlight.direction = glm::vec3(1.0f,1.0f,1.0f);

    flashlight.position = glm::vec3(0.0f,0.0f,3.0f);
    flashlight.direction = glm::vec3(0.0f,0.0f,0.0f);
    flashlight.color = glm::vec3(1.0f,1.0f,1.0f);

    flashlight.cutOffangle = glm::cos(glm::radians(12.5f));
    flashlight.outerCutOff = glm::cos(glm::radians(17.5f));

    flashlight.constant = 1.0f;
    flashlight.linear = 0.001f;
    flashlight.quadratic = 0.016f;
}

void setPointLight(const unsigned int &shaderProgram, const std::string &target, const pointLight &pl) {

    setVec3(shaderProgram, (target + ".position").c_str(), pl.position);
    setVec3(shaderProgram, (target + ".color").c_str(), pl.color);

    setFloat(shaderProgram, (target + ".constant").c_str(), pl.constant);
    setFloat(shaderProgram, (target + ".linear").c_str(), pl.linear);
    setFloat(shaderProgram, (target + ".quadratic").c_str(), pl.quadratic);
}

void setSpotLight(const unsigned int &shaderProgram, const std::string &target, const spotLight &sl) {

    setVec3(shaderProgram, (target + ".position").c_str(), sl.position);
    setVec3(shaderProgram, (target + ".direction").c_str(), sl.direction);
    setVec3(shaderProgram, (target + ".color").c_str(), sl.color);

    setFloat(shaderProgram, (target + ".cutOffangle").c_str(), sl.cutOffangle);
    setFloat(shaderProgram, (target + ".outerCutOff").c_str(), sl.outerCutOff);

    setFloat(shaderProgram, (target + ".constant").c_str(), sl.constant);
    setFloat(shaderProgram, (target + ".linear").c_str(), sl.linear);
    setFloat(shaderProgram, (target + ".quadratic").c_str(), sl.quadratic);
}

void setDirectionalLight(const unsigned int &shaderProgram, const std::string &target, const directionalLight &dl) {

    setVec3(shaderProgram, (target + ".direction").c_str(), dl.direction);
    setVec3(shaderProgram, (target + ".color").c_str(), dl.color);
}

colors::colors() {

    red = glm::vec3(1.0f,0.0f,0.0f);
    orange = glm::vec3(0.945f,0.352f,0.133f);
    yellow = glm::vec3(1.0f,1.0f,0.0f);
    green = glm::vec3(0.0f,1.0f,0.0f);
    blue = glm::vec3(0.0f,0.0f,1.0f);
    pink = glm::vec3(0.89f,0.239f,0.58f);
    white = glm::vec3(1.0f,1.0f,1.0f);
    black = glm::vec3(0.0f,0.0f,0.0f);
    gray = glm::vec3(0.392f,0.392f,0.392f);

}

//-------------------------------------------------------------------------------------//

lightSource::lightSource() {

    src = lights::instance().cubelight;
    srcShape = shapes::instance().cube;
}

void lightSource::setLightColor(const glm::vec3& color) {
    src.color = color;
}

void lightSource::setPosition(const glm::vec3& position) {
    src.position = position;
}

void lightSource::draw(const unsigned int& shader) const {

    glBindVertexArray(srcShape.VAO);
    glDrawElements(GL_TRIANGLES, srcShape.indicesCount, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));
    glBindVertexArray(0);
}