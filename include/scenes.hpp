#ifndef SCENES_HPP
#define SCENES_HPP

#include <shader.hpp>

class scene1 {

	unsigned int &lineShader, &lightShader, &planeShader, &modelShader, &textureShader;

public:

	void init();
	void update();
	void render() const;

	void destroy();
};

#endif