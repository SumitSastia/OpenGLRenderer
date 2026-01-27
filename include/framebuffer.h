#pragma once

#include <shader.h>

struct HDR_frame {

	unsigned int fbo, rbo, vbo, vao;
	unsigned int texture_id;
	unsigned int shader;

	HDR_frame(const int& frameWidth, const int& frameHeight);

	void init();
	void render() const;
};