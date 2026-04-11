#include <iostream>
#include <framebuffer.h>
#include <lights.h>
#include <camera.h>
#include <random>

unsigned int gfx::internal::screen::vbo = 0;
unsigned int gfx::internal::screen::vao = 0;

namespace frameBuffers {
    
    const unsigned int get_defaultVAO() {
        return gfx::internal::screen::instance().vao;
    }

    HDR_frame::HDR_frame(const int& frameWidth, const int& frameHeight) {

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Texture Attachment
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        this->init();
    }

    void HDR_frame::init() {

        shader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/frame_buffer.frag"
        );
    }

    void HDR_frame::render() const {

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);
        setBool(shader, "normalRender", true);
        setMat4(shader, "model", glm::mat4(1.0f));

        glBindVertexArray(get_defaultVAO());

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //-------------------------------------------------------------------------------------//

    bloom_frame::bloom_frame(const int& frameWidth, const int& frameHeight) :
        _blur_frame(frameWidth / 1.5, frameHeight / 1.5) {

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
            std::cerr << "ERROR :: UNABLE TO COMPLETE BLOOM-FRAME-BUFFER!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        this->init();
    }

    void bloom_frame::init() {

        shader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/bloom.frag"
        );

        blurShader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/blur.frag"
        );
    }

    void bloom_frame::render() const {

        bool horizontal = true;
        bool first_itr = true;
        int amount = 10;

        glUseProgram(blurShader);

        for (unsigned int i = 0; i < amount; i++) {

            glBindFramebuffer(GL_FRAMEBUFFER, _blur_frame.fbo[horizontal]);
            setBool(blurShader, "horizontal", horizontal);

            glBindTexture(GL_TEXTURE_2D, first_itr ? colorBuffers[1] : _blur_frame.texture_buffers[!horizontal]);

            glBindVertexArray(get_defaultVAO());
            glDrawArrays(GL_TRIANGLES, 0, 6);

            horizontal = !horizontal;
            first_itr = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        setInt(shader, "screen", 0);
        setInt(shader, "bloom", 1);

        glBindVertexArray(get_defaultVAO());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _blur_frame.texture_buffers[0]);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //-------------------------------------------------------------------------------------//

    blur_frame::blur_frame(const int& frameWidth, const int& frameHeight) {

        glGenFramebuffers(2, fbo);
        glGenTextures(2, texture_buffers);

        for (unsigned int i = 0; i < 2; i++) {

            glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
            glBindTexture(GL_TEXTURE_2D, texture_buffers[i]);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_buffers[i], 0);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR :: UNABLE TO COMPLETE BLUR-FRAME-BUFFER!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
            std::cerr << "ERROR :: UNABLE TO COMPLETE DEFAULT-FRAME-BUFFER!" << std::endl;
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

        glBindVertexArray(get_defaultVAO());

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

    // Deferred Shading
    g_buffer::g_buffer(const int& frameWidth, const int& frameHeight) {

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &gPosition);
        glGenTextures(1, &gNormal);
        glGenTextures(1, &gTexture);
        glGenTextures(1, &noiseTexture);

        // Position Buffer
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
        
        // Normal Buffer
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        // Texture Buffer
        glBindTexture(GL_TEXTURE_2D, gTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, frameWidth, frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gTexture, 0);

        attachments[0] = GL_COLOR_ATTACHMENT0;
        attachments[1] = GL_COLOR_ATTACHMENT1;
        attachments[2] = GL_COLOR_ATTACHMENT2;

        glDrawBuffers(3, attachments);

        // Render Object
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameWidth, frameHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: G-Buffer incomplete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // SSAO Initialization

        glGenFramebuffers(1, &ssao_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);

        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WIN_W, WIN_H, 0, GL_RED, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "SSAO FBO incomplete!" << std::endl;
        }

        this->init();
        this->initSSAO();
    }

    void g_buffer::init() {

        defaultShader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/deferred.frag"
        );

        shader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/ssao.frag"
        );

        ssaoShader = createShader(
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/default_fb.vert",
            "C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/framebfs/ssao_final.frag"
        );
    }

    void g_buffer::renderDefault() const {

        glUseProgram(defaultShader);

        setInt(defaultShader, "gPosition", 0);
        setInt(defaultShader, "gNormal", 1);
        setInt(defaultShader, "gTexture", 2);

        glBindVertexArray(get_defaultVAO());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gTexture);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void g_buffer::render() const {

        glUseProgram(shader);

        setInt(shader, "gPosition", 0);
        setInt(shader, "gNormal", 1);
        setInt(shader, "gTexture", 2);
        setInt(shader, "ssaoTexture", 3);

        glBindVertexArray(get_defaultVAO());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gTexture);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void g_buffer::initSSAO() {

        std::uniform_real_distribution <float> randomFloats(0.0, 1.0);
        std::default_random_engine generator;

        for (unsigned int i = 0; i < 64; i++) {

            glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
            );

            sample = glm::normalize(sample);
            sample *= randomFloats(generator);

            float scale = float(i) / 64.0f;
            scale = lerp(0.1f, 1.0f, scale * scale);

            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        std::vector <glm::vec3> ssaoNoise;

        for (unsigned int i = 0; i < 16; i++) {

            glm::vec3 noise(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f
            );

            ssaoNoise.push_back(noise);
        }

        // Noise Buffer
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    void g_buffer::renderSSAO() const {

        glBindFramebuffer(GL_FRAMEBUFFER, ssao_fbo);
        glViewport(0, 0, WIN_W, WIN_H);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ssaoShader);

        // Send Kernal Rotations
        for (unsigned int i = 0; i < 64; i++) {
            setVec3(ssaoShader, ("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
        }

        setMat4(ssaoShader, "projection", camera::instance().getPerspective());
        setMat4(ssaoShader, "view", camera::instance().getView());

        setInt(ssaoShader, "gPosition", 0);
        setInt(ssaoShader, "gNormal", 1);
        setInt(ssaoShader, "noiseTexture", 2);

        glBindVertexArray(get_defaultVAO());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}