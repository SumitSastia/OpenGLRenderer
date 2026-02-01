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

	struct bloom_frame : public frameBuffer {

		unsigned int colorBuffers[2];
		unsigned int attachments[2];

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
}