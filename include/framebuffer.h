#pragma once

#include <shader.h>

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

struct default_frame : public frameBuffer {

	default_frame(const int& frameWidth, const int& frameHeight);

	void init() override;
	void render() const override;
};

struct pointShadow_frame {

	unsigned int fbo;
	unsigned int texture_id;

	pointShadow_frame(const int& shadowWidth, const int& shadowHeight);
};