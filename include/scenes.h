#ifndef SCENES_HPP
#define SCENES_HPP

#include <shader.h>
#include <lights.h>
#include <framebuffer.h>

class scene1 {

	// Shaders
	unsigned int
		lineShader,
		lightShader,
		planeShader,
		modelShader,
		textureShader,
		cubemapShader,
		normalShader,
		instanceShader,
		instanceShadowShader,
		shadowShader,
		pointShadowPlanes,
		pointShadowInstanced
	;

	unsigned int
		pointShadow_frame_shader;
	;

	// Light Space
	glm::mat4 lightSpace;
	glm::mat4 lightProjection;
	glm::mat4 shadowProj;

	// Imported Models
	model3D* cube1;

	// Light-Source
	unsigned int lights_count;
	std::vector <lightSource*> lights;

	lightSource* myLight;
	lightSource* light2;

	// World Space Models
	glm::mat4 objectModel;
	glm::mat4 lightModel;
	glm::mat4 lightModel2;
	glm::mat4 cubeModel;
	glm::mat4 floorModel;
	glm::mat4 windowModel;

	unsigned int modelCounter;
	std::vector <glm::mat4*> worldModels;

	// Skybox
	cubeMap* skybox;
	bool skybox_isVisible;
	float skyboxIntensity;

	// Multiple Cubes
	unsigned int totalCubes;
	unsigned int instanceModelVBO, instanceNormalVBO;
	glm::vec3* cubePositions;

	// ColoredCube
	coloredCube* cc1;

	// Floor
	shape2D floor;

	// FrameBuffers
	std::vector <pointShadow_frame*> light_frames;

	// Developer Tools

public:

	scene1() : modelCounter(0) {}

	model3D* getCube() const { return cube1; }
	std::vector <lightSource*> getLights() const { return lights; }

	void init();
	void input_handler(GLFWwindow* window);

	void update(const float& delta_time);

	void render() const;
	void render_transparent() const;

	void render_shadow(const unsigned int& shader) const;
	void render_pointShadow() const;

	void render_with_pointLight() const;

	void destroy() const;
};

#endif