#include <scenes.hpp>

void scene1::init() {

    shader s1(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\lines.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\lines.frag"
    );

    shader s2(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\light.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\light.frag"
    );

    shader s3(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\texture.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\texture.frag"
    );

    shader s4(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\model.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\model.frag"
    );

    shader s5(
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\planes.vert",
        "C:\\Users\\sumit\\Documents\\GitHub\\OpenGLRenderer\\shaders\\planes.frag"
    );

    lineShader = s1.get_program();
    lightShader = s2.get_program();
    textureShader = s3.get_program();
    modelShader = s4.get_program();
    planeShader = s5.get_program();
}


void scene1::destroy() {

    glDeleteProgram(lineShader);
    glDeleteProgram(lightShader);
    glDeleteProgram(textureShader);
    glDeleteProgram(modelShader);
    glDeleteProgram(planeShader);
}