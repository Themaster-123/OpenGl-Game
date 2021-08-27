#include <algorithm>
#include "screen.h"
#include "objects/entities/entity.h"
#include <iostream>



void glg::loopThroughEntitys() {
	std::cout << getEntityUpdateVector().size() << std::endl;
	for (Entity* entity : getEntityUpdateVector()) {
		(*entity).update();
	}
}

void glg::addEntityToUpdateCycle(glg::Entity& entity) {
	getEntityUpdateVector().push_back(&entity);
	std::cout << getEntityUpdateVector().size() << std::endl;
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

	return window;
}

void glg::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, width, height);
}