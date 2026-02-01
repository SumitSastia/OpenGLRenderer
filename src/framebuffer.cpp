#include <iostream>
#include <framebuffer.h>

namespace frameBuffers {

    HDR_frame::HDR_frame(const int& frameWidth, const int& frameHeight) {

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Texture Attachment
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

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

    //-------------------------------------------------------------------------------------//

    bloom_frame::bloom_frame(const int& frameWidth, const int& frameHeight) {

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Texture Attachment
        glGenTextures(2, colorBuffers);

        for (unsigned int i = 0; i < 2; i++) {

            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }

        // Render Object
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        attachments[0] = GL_COLOR_ATTACHMENT0;
        attachments[1] = GL_COLOR_ATTACHMENT1;
 
        glDrawBuffers(2, attachments);

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

    void bloom_frame::init() {

        shader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/bloom.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/bloom.frag"
        );
    }

    void bloom_frame::render() const {

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        setInt(shader, "screen", 0);
        setInt(shader, "bloom", 1);

        glBindVertexArray(vao);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[1]);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //-------------------------------------------------------------------------------------//

    default_frame::default_frame(const int& frameWidth, const int& frameHeight) {

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Texture Attachment
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

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

    void default_frame::init() {

        shader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.frag"
        );
    }

    void default_frame::render() const {

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        setBool(shader, "normalRender", true);
        setMat4(shader, "model", glm::mat4(1.0f));

        glBindVertexArray(vao);

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //-------------------------------------------------------------------------------------//

    pointShadow_frame::pointShadow_frame(const int& shadowWidth, const int& shadowHeight) {

        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &texture_id);

        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        for (unsigned int i = 0; i < 6; i++) {

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_id, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: Cubemap shadow framebuffer incomplete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}