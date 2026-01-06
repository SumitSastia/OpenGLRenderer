#ifndef SCENES_HPP
#define SCENES_HPP

#include <shader.hpp>
#include <lights.hpp>

class scene1 {

	// Objects
	unsigned int lineShader, lightShader, planeShader, modelShader, textureShader;
	model3D* cube1;

	// Light-Source
	lightSource* myLight;

	// World Space Models
	glm::mat4 objectModel;
	glm::mat4 lightModel;
	glm::mat4 cubeModel;

public:

	void init();
	void update(const float& delta_time);
	void render() const;

	void destroy() const;
};

#endif