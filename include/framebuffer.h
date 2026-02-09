#pragma once

#include <shader.h>

#define shadowSize 1024

namespace frameBuffers {

	struct frameBuffer {

		unsigned int fbo, rbo, vbo, vao;
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

		unsigned int fbo, rbo, vbo, vao;
		unsigned int gPosition, gNormal, gTexture;

		unsigned int attachments[3];
		unsigned int shader;

		g_buffer(const int& frameWidth, const int& frameHeight);

		void init();
		void render() const;

		const unsigned int get_shader() const { return shader; }
	};
}