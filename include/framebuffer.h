#pragma once

#include <shader.h>
#include <iostream>
#include <vector>

#define shadowSize 1024

namespace gfx::internal {

	class screen {
		
	public:
		static unsigned int vbo, vao;

		screen() {
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

		static screen& instance() {

			static screen instance {};
			return instance;
		}
	};
}

namespace frameBuffers {

	const unsigned int get_defaultVAO();

	struct frameBuffer {

		unsigned int fbo, rbo;
		unsigned int texture_id;
		unsigned int shader;

		virtual void init() {}
		virtual void render() const {}
	};

	struct HDR_frame : public frameBuffer {

		HDR_frame(const int& frameWidth, const int& frameHeight);

		void init();
		void render() const;
	};

	struct blur_frame {

		unsigned int fbo[2];
		unsigned int texture_buffers[2];

		blur_frame(const int& frameWidth, const int& frameHeight);
	};

	struct bloom_frame : public frameBuffer {

		unsigned int colorBuffers[2];
		unsigned int attachments[2];

		blur_frame _blur_frame;
		unsigned int blurShader;

		bloom_frame(const int& frameWidth, const int& frameHeight);

		void init() override;
		void render() const override;
	};

	struct default_frame : public frameBuffer {

		default_frame(const int& frameWidth, const int& frameHeight);

		void init() override;
		void render() const override;
	};

	struct pointShadow_frame {

		unsigned int fbo;
		unsigned int texture_id;

		pointShadow_frame(const int& shadowWidth, const int& shadowHeight);
		pointShadow_frame() : pointShadow_frame(shadowSize, shadowSize) {}
	};

	// Deferred Shading
	struct g_buffer {

		// Deferred
		unsigned int fbo, rbo;
		unsigned int gPosition, gNormal, gTexture;

		unsigned int attachments[3];
		unsigned int shader;
		unsigned int defaultShader;

		// SSAO
		unsigned int ssao_fbo;
		unsigned int ssaoColorBuffer, noiseTexture;
		unsigned int ssaoShader;

		std::vector <glm::vec3> ssaoKernel;

		g_buffer(const int& frameWidth, const int& frameHeight);

		void init();
		void initSSAO();
		void render() const;
		void renderSSAO() const;

		void renderDefault() const;

		int lerp(float a, float b, float f) const {
			return (a + f * (b - a));
		}

		const unsigned int get_shader() const { return shader; }
		const unsigned int get_defaultShader() const { return defaultShader; }
	};
}