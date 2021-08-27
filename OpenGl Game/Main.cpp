#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "texture.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility>

using namespace GLG;

unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 800;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

float deltaTime = 0;
float lastFrame = 0;

glm::vec3 cameraPos = glm::vec3(0, 0, 3);
glm::vec3 cameraFront = glm::vec3(0, 0, -1);
glm::vec3 cameraUp = glm::vec3(0, 1, 0);

float yaw = 0;
float pitch = 0;

Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), 90, SCREEN_WIDTH / SCREEN_HEIGHT);

float lastMouseX;
float lastMouseY;

bool firstMouse = true;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

GLFWwindow* createWindow(int width, int height, const char* title);

void loadOpenGlFunctions();

void calculateDeltaTime();

void lockCursor(GLFWwindow* window, bool locked);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Texture2D texture1("Assets/crate.jpg", 0, WrappingOption::Repeat);
	Texture2D texture2("Assets/awesomeface.png", 1, WrappingOption::Repeat);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	glm::vec3 cubePositions[] = {
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");	

	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		calculateDeltaTime();
		processInput(window);

		glClearColor(.3f, 0, .2f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		shader.use();
		texture1.activate();
		texture2.activate();
		glBindVertexArray(VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 view = camera.getViewMatrix();
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader.setMat4("view", view);

		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() / 4 + glm::radians(i * 20.0f), glm::vec3(.5, .5, 0));
			shader.setMat4("model", model);
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
			glDrawArrays(GL_TRIANGLES, 0, 36/*6, GL_UNSIGNED_INT, 0*/);
		}


		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	const float cameraSpeed = 3 * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.setPosition(camera.getPosition() + glm::normalize(camera.getFront()) * cameraSpeed);
	} 
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.setPosition(camera.getPosition() - glm::normalize(camera.getFront()) * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.setPosition(camera.getPosition() - glm::normalize(camera.getRight()) * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.setPosition(camera.getPosition() + glm::normalize(camera.getRight()) * cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.setPosition(camera.getPosition() + glm::normalize(camera.getUp()) * cameraSpeed);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastMouseX;
	float yOffset = lastMouseY - ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;

	const float sensitivity = 0.2;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	yaw += xOffset;
	pitch += yOffset;
	pitch = std::max(std::min(pitch, 90.0f), -90.0f);
	cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(cameraFront);
	camera.setRotation(glm::vec3(pitch, -yaw, 0));
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
	glfwSetCursorPosCallback(window, mouse_callback);
	lastMouseX = width / 2;
	lastMouseY = height / 2;

	return window;
}

void loadOpenGlFunctions() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("No glad just sad");
	}
}

void calculateDeltaTime() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void lockCursor(GLFWwindow* window, bool locked) {
	int disabled;
	if (locked) {
		disabled = GLFW_CURSOR_DISABLED;
	}
	else {
		disabled = GLFW_CURSOR_NORMAL;
	}
	glfwSetInputMode(window, GLFW_CURSOR, disabled);
}

//void setTextureSetting() {
//	glTexParameteri(GL_TEXTURE_2D)
//}


