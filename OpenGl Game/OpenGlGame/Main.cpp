#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "resources/shader.h"
#include "resources/texture.h"
#include "entities/camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility>
#include <vector>
#include "screen.h"
#include "entities/Alive/player.h"
#include "resources/model.h"
#include "entities/visible_entity.h"
#include "globals/shaders.h"
#include "globals/models.h"
#include <reactphysics3d/reactphysics3d.h>
#include "physics.h"

using namespace glg;

Camera camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 70, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);

void loadOpenGlFunctions();

int main() {
	// initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	// window creation
	GLFWwindow* window = createWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test");
	lockCursor(window, true);
	// load all OpenGl function pointers
	loadOpenGlFunctions();
	shaders::registerShaders();
	models::registerModels();
	registerPhysics();

	//shader.use();

	Player player(camera.getPosition(), camera.getRotation(), camera);


	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		calculateDeltaTime();

		glClearColor(.3f, 0, .2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);

		player.camera.aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;


		glm::mat4 view = player.camera.getViewMatrix();

		shaders::defaultShader.setMat4("view", view);
		shaders::defaultShader.setMat4("projection", player.camera.getProjectionMatrix());
		glm::mat4 modeli = glm::mat4(1);
		shaders::defaultShader.setMat4("model", modeli);
		models::defaultModel.draw(shaders::defaultShader);

		loopThroughEntitys();

		//player.draw();
		//visibleEntity.draw();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		physicsFrame();

		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void loadOpenGlFunctions() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("No glad just sad");
	}
}

//void setTextureSetting() {
//	glTexParameteri(GL_TEXTURE_2D)
//}


