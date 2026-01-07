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

	model3D* getCube() const { return cube1; }
	lightSource* getLight() const { return myLight; }

	void init();
	void update(const float& delta_time);
	void render() const;

	void destroy() const;
};

#endif