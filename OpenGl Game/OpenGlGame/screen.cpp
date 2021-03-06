#include "screen.h"
#include <glad/glad.h>
#include <algorithm>
#include "scene.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "physics.h"
#include <entt/entt.hpp>
#include "components/components.h"
#include "component_systems/component_system.h"
#include "component_systems/transform_system/transform_system.h"
#include <thread>

GLFWwindow* glg::GAME_WINDOW;
unsigned int glg::SCREEN_WIDTH = 800;
unsigned int glg::SCREEN_HEIGHT = 800;
float glg::DELTA_TIME;
float glg::FRAME_RATE;
float glg::FRAME_RATE_SMOOTHING = .9f;
static float lastMouseX;
static float lastMouseY;

void glg::callDraw()
{
	auto& systems = Scene::getGlobalSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->globalDraw();

		for (Scene* scene : Scene::SCENES) {
			systems[i]->draw(scene);
		}
	}
}

void glg::callUpdate() {
	auto& systems = Scene::getGlobalSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->globalUpdate();

		for (Scene* scene : Scene::SCENES) {
			systems[i]->update(scene);
		}
	}
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

	auto& systems = Scene::getGlobalSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->globalOnMouseMovement(xOffset, yOffset, (float)xpos, (float)ypos);

		for (Scene* scene : Scene::SCENES) {
			systems[i]->onMouseMovement(xOffset, yOffset, (float)xpos, (float)ypos, scene);
		}
	}
}

void glg::calculateDeltaTime() {
	static float lastFrame = 0;

	float currentFrame = (float)glfwGetTime();
	DELTA_TIME = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void glg::calculateFrameRate()
{
	FRAME_RATE = (FRAME_RATE * FRAME_RATE_SMOOTHING) + (1 / glg::DELTA_TIME * (1 - FRAME_RATE_SMOOTHING));
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
		calculateFrameRate();

		glClearColor(95.0f / 255, 203.0f / 255, 243.0f / 255, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		physicsFrame();

		callUpdate();

		callDraw();
		std::cout << "FPS: " << FRAME_RATE << std::endl;

		glBindVertexArray(0);

		glfwSwapBuffers(GAME_WINDOW);
		glfwPollEvents();
	}
}
