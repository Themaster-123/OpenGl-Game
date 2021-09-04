#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "entities/entity.h"
#include "entities/Alive/player.h"

namespace glg  {
	extern GLFWwindow* GAME_WINDOW;
	extern unsigned int SCREEN_WIDTH;
	extern unsigned int SCREEN_HEIGHT;
	extern float DELTA_TIME;

	void loopThroughEntitys();

	void loopThroughEntitysPhysics();

	void addEntityToUpdateCycle(glg::Entity& entity);

	void removeEntityFromUpdateCycle(const glg::Entity& entity);

	std::vector<Entity*>& getEntityUpdateVector();

	GLFWwindow* createWindow(int width, int height, const char* title);

	void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void calculateDeltaTime();

	void lockCursor(GLFWwindow* window, bool locked);

	int getKey(int key);

	void startRenderLoop();
}
