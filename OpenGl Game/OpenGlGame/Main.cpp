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

using namespace glg;

Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 70, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);

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

	Shader shader("Assets/VertexShader.vert", "Assets/FragmentShader.frag");

	shader.use();

	Player player(camera.getPosition(), camera.getRotation(), camera);


	glEnable(GL_DEPTH_TEST);
	shader.use();
	//Model model("assets/test.fbx");
	Model model("assets/backpack/backpack.obj");
	Model model2(model);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		calculateDeltaTime();
		loopThroughEntitys();

		glClearColor(.3f, 0, .2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		//shader.use();
		//texture1.activate();
		//texture2.activate();
		glm::mat4 modeli = glm::mat4(1);
		modeli = glm::rotate(modeli, (float)glfwGetTime(), glm::vec3(1, 1, 0));
		shader.setMat4("model", modeli);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		camera.aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
		glm::mat4 view = player.camera.getViewMatrix();
		shader.setMat4("view", view);
		shader.setMat4("projection", player.camera.getProjectionMatrix());
		model.draw(shader);
		modeli = glm::mat4(1);
		modeli = glm::translate(modeli, glm::vec3(0, 5, 0));
		modeli = glm::rotate(modeli, (float)glfwGetTime(), glm::vec3(1, 1, 0));
		shader.setMat4("model", modeli);
		model2.draw(shader);
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		
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


