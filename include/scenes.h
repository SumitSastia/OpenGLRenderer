#pragma once

#include <shader.h>
#include <lights.h>
#include <framebuffer.h>

namespace scenes {

	class scene_base {

	protected:

		unsigned int light_shader; // For rendering Lights
		unsigned int pointShadowFrame_shader; // For rendering Shadows

		// Light-Source
		unsigned int lights_count;
		std::vector <lights::lightSource*> lights;
		std::vector <frameBuffers::pointShadow_frame*> light_frames;

	public:

		scene_base() :
			lights_count(0) {

			light_shader = createShader(
				"C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.vert",
				"C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/light.frag"
			);

			pointShadowFrame_shader = createShader2(
				"C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.vert",
				"C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.geom",
				"C:/Users/sumit/Documents/GitHub/OpenGLRenderer/shaders/pointShadow/shadow.frag"
			);
		}

		virtual void init() {};
		virtual void update() {};
		virtual void render() const {};
	};

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
			pointShadowInstanced,
			gbufferShader,
			gbufferInstanced,
			gbufferPlanes
		;

		unsigned int
			pointShadow_frame_shader;
		;

		// Light Space
		glm::mat4 lightSpace;
		glm::mat4 lightProjection;
		glm::mat4 shadowProj;

		// Imported Models
		models::model3D* cube1;

		// Light-Source
		unsigned int lights_count;
		std::vector <lights::lightSource*> lights;

		lights::lightSource* myLight;
		lights::lightSource* light2;

		// World Space Models
		glm::mat4 objectModel;
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
		std::vector <frameBuffers::pointShadow_frame*> light_frames;

		// Developer Tools
		texture wood;

	public:

		scene1() : modelCounter(0) {

			wood.load("C:/Users/sumit/Documents/GitHub/OpenGLRenderer/assets/textures/wood_box.png");
		}

		models::model3D* getCube() const { return cube1; }
		std::vector <lights::lightSource*> getLights() const { return lights; }
		unsigned int getGbufferShader() const { return gbufferShader; }

		void init();
		void input_handler(GLFWwindow* window);

		void update(const float& delta_time);

		void render() const;
		void render_transparent() const;

		void render_shadow(const unsigned int& shader) const;
		void render_pointShadow() const;
		
		// Forward Rendering
		void render_with_pointLight() const;

		// Deferred Rendering
		void render_lights() const;
		void render_g_buffer() const;
		void render_final(const frameBuffers::g_buffer* _g_buffer) const;
		void renderSSAO(const frameBuffers::g_buffer* _g_buffer) const;

		void destroy() const;
	};

	class scene2 : public scene_base {

		unsigned int textureCube_Shader, gbufferPlanes, gbuffer3D;

		// Floor
		shape2D floor;

		specShape wall;

		glm::mat4 objectModel;
		glm::mat4 floorModel;
		glm::mat4 wallModel;
		glm::mat4 wallModel2;

		glm::mat4 cubeModel0;
		glm::mat4 cubeModel1;

		glm::mat4 shadowProj;

	public:

		scene2() {
			this->init();
		}

		void init();
		void input_handler();

		void update(const float& delta_time);

		void render();
		void render_pointShadow() const;
		void render_final(const frameBuffers::g_buffer* _g_buffer) const;

		void destroy();
	};
}