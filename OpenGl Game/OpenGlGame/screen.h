#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "objects/entities/entity.h"

namespace glg  {
	extern const GLFWwindow* GAME_WINDOW;

	void loopThroughEntitys();

	void addEntityToUpdateCycle(glg::Entity& entity);

	void removeEntityFromUpdateCycle(const glg::Entity& entity);

	std::vector<Entity*>& getEntityUpdateVector();

	GLFWwindow* createWindow(int width, int height, const char* title);

	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
}
