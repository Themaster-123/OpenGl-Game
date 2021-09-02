#include "screen.h"
#include <glad/glad.h>
#include <algorithm>
#include "entities/entity.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "physics.h"

GLFWwindow* glg::GAME_WINDOW;
unsigned int glg::SCREEN_WIDTH = 800;
unsigned int glg::SCREEN_HEIGHT = 800;
float glg::DELTA_TIME;
static float lastMouseX;
static float lastMouseY;

void glg::loopThroughEntitys() {
	for (Entity* entity : getEntityUpdateVector()) {
		(*entity).update();
	}
}

void glg::loopThroughEntitysPhysics()
{
	for (Entity* entity : getEntityUpdateVector()) {
		(*entity).physicsUpdate();
	}
}

void glg::addEntityToUpdateCycle(glg::Entity& entity) {
	getEntityUpdateVector().push_back(&entity);
}

void glg::removeEntityFromUpdateCycle(const glg::Entity& entity) {
	int eraseIndex = -1;
	for (int i = 0; i < getEntityUpdateVector().size(); i++) {
		if (&entity == getEntityUpdateVector()[i]) {
			eraseIndex = i;
		}
	}
	getEntityUpdateVector().erase(getEntityUpdateVector().begin() + eraseIndex);
}

std::vector<glg::Entity*>& glg::getEntityUpdateVector() {
	static std::vector<Entity*> vector;
	return vector;
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

	for (Entity* entity : getEntityUpdateVector()) {
		(*entity).onMouseMovement(xOffset, yOffset, (float) xpos, (float) ypos);
	}

	//const float sensitivity = 0.2f;
	//xOffset *= sensitivity;
	//yOffset *= sensitivity;
	//glm::vec3 euler = glm::degrees(glm::eulerAngles(camera.getRotation()));
	//euler.y -= xOffset;
	//euler.x += yOffset;
	//euler.x = std::max(std::min(euler.x, 90.0f), -90.0f);
	//camera.setRotation(euler);
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

		glm::mat4 modeli = glm::mat4(1);
		//models::defaultModel.draw(shaders::defaultShader);

		loopThroughEntitys();

		//player.draw();
		//visibleEntity.draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		glBindVertexArray(0);

		glfwSwapBuffers(GAME_WINDOW);
		glfwPollEvents();
	}
}
