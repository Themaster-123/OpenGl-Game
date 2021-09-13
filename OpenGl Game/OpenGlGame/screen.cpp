#include "screen.h"
#include <glad/glad.h>
#include <algorithm>
#include "scene.h"
#include "entities/entity.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "physics.h"
#include "world/world.h"
#include "entities/Alive/player.h"
#include <entt/entt.hpp>
#include "components/components.h"
#include "component_systems/component_system.h"
#include "component_systems/transform_system/transform_system.h"

GLFWwindow* glg::GAME_WINDOW;
unsigned int glg::SCREEN_WIDTH = 800;
unsigned int glg::SCREEN_HEIGHT = 800;
float glg::DELTA_TIME;
static float lastMouseX;
static float lastMouseY;

void glg::drawEntities()
{
	for (int i = 0; i < scene::getEntities().size(); i++) {
		VisibleEntity* entity = dynamic_cast<VisibleEntity*>(scene::getEntities()[i]);
		if (entity != nullptr) {
			entity->draw();
		}
	}

	auto systems = scene::getSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->draw();
	}
}

void glg::loopThroughEntities() {
	for (int i = 0; i < scene::getEntities().size(); i++) {
		Entity* entity = scene::getEntities()[i];
		(*entity).update();
	}

	auto systems = scene::getSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->update();
	}
}

void glg::addEntityToUpdateCycle(glg::Entity& entity) {
	scene::getEntities().push_back(&entity);
}

void glg::removeEntityFromUpdateCycle(const glg::Entity& entity) {
	int eraseIndex = -1;
	for (int i = 0; i < scene::getEntities().size(); i++) {
		if (&entity == scene::getEntities()[i]) {
			eraseIndex = i;
		}
	}
	scene::getEntities().erase(scene::getEntities().begin() + eraseIndex);
}

GLFWwindow* glg::createWindow(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		//std::cout << "LOZER ha ha you failed to create a window STUPID" << std::endl;
		throw std::runtime_error("LOZER ha ha you failed to create a window STUPID");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouse_callback);
	lastMouseX = width / 2.0f;
	lastMouseY = height / 2.0f;
	GAME_WINDOW = window;
	return window;
}

void glg::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void glg::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;

	if (firstMouse) {
		lastMouseX = (float)xpos;
		lastMouseY = (float)ypos;
		firstMouse = false;
	}

	float xOffset = (float)xpos - lastMouseX;
	float yOffset = (float)ypos - lastMouseY;
	lastMouseX = (float)xpos;
	lastMouseY = (float)ypos;

	for (Entity* entity : scene::getEntities()) {
		(*entity).onMouseMovement(xOffset, yOffset, (float) xpos, (float) ypos);
	}

	auto systems = scene::getSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->onMouseMovement(xOffset, yOffset, (float)xpos, (float)ypos);
	}
}

void glg::calculateDeltaTime() {
	static float lastFrame = 0;

	float currentFrame = (float)glfwGetTime();
	DELTA_TIME = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void glg::lockCursor(GLFWwindow* window, bool locked) {
	int disabled;
	if (locked) {
		disabled = GLFW_CURSOR_DISABLED;
	}
	else {
		disabled = GLFW_CURSOR_NORMAL;
	}
	glfwSetInputMode(window, GLFW_CURSOR, disabled);
}

int glg::getKey(int key)
{
	return glfwGetKey(GAME_WINDOW, key);
}

void glg::startRenderLoop()
{
	while (!glfwWindowShouldClose(GAME_WINDOW))
	{
		calculateDeltaTime();

		glClearColor(.3f, 0, .2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		physicsFrame();

		loopThroughEntities();

		setLightsUniforms();

		drawEntities();

		//obj.get<ModelComponent>().draw();

		glBindVertexArray(0);

		glfwSwapBuffers(GAME_WINDOW);
		glfwPollEvents();
	}
}

void glg::setLightsUniforms()
{
	for (Shader* shader : shaders::getShaders()) {
		shader->setInt("lightsSize", scene::getLights().size());
		for (int i = 0; i < scene::getLights().size(); i++) {
			//scene::getLights()[i]->setShaderLightUniforms(shader, i);
		}
	}
}
