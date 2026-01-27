#include <iostream>
#include <framebuffer.h>

HDR_frame::HDR_frame(const int& frameWidth, const int& frameHeight) {

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Texture Attachment
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

    // Render Object
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR :: UNABLE TO COMPLETE HDR-FRAME-BUFFER!" << std::endl;
    }
    else {

        // Vertices Binding
        const float vertices[] = {

            // Position  // Cords
            -1.0f, 1.0f, 0.0f, 1.0f,
             1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 0.0f, 0.0f,
                               
             1.0f, 1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f, 0.0f,
            -1.0f,-1.0f, 0.0f, 0.0f
        };

        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    this->init();
}

void HDR_frame::init() {

    shader = createShader(
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/frame_buffer.vert",
        "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/frame_buffer.frag"
    );
}

void HDR_frame::render() const {

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);
    setBool(shader, "normalRender", true);
    setMat4(shader, "model", glm::mat4(1.0f));

    glBindVertexArray(vao);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}