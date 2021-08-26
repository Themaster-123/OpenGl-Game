#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "texture.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace GLG;

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;
float vertices[] = {
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	// top left
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

GLFWwindow* createWindow(int width, int height, const char* title);

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
	GLFWwindow* window = createWindow(SCREEN_HEIGHT, SCREEN_HEIGHT, "Test");

	// load all OpenGl function pointers
	loadOpenGlFunctions();

	Shader shader("Assets/VertexShader.vert", "Assets/FragmentShader.frag");

	// creates a vertex array object
	// 
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// creates a vertex buffer object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// creates a element buffer object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//links vertex attributes to the VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Texture2D texture1("Assets/crate.jpg", 0, WrappingOption::Repeat, FilterOption::Linear);
	Texture2D texture2("Assets/awesomeface.png", 1, WrappingOption::Repeat, FilterOption::Linear);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);
	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(.3f, 0, .2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glm::mat4 transform = glm::mat4(1);
		//transform = glm::translate(transform, glm::vec3(.5f, -.5f, 0));
		transform = glm::scale(transform, glm::vec3(2, sin((float)glfwGetTime()), .5));
		//transform = glm::rotate(transform, , glm::vec3(1, 0, 0));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		texture1.activate();
		texture2.activate();
		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* createWindow(int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		//std::cout << "LOZER ha ha you failed to create a window STUPID" << std::endl;
		throw std::runtime_error("LOZER ha ha you failed to create a window STUPID");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	return window;
}

void loadOpenGlFunctions() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("No glad just sad");
	}
}

//void setTextureSetting() {
//	glTexParameteri(GL_TEXTURE_2D)
//}


