#include "player.h"
#include <utility>
#include "../../screen.h"

using namespace glg;

Player::Player(glm::vec3 position, glm::quat rotation, const Camera& camera, float sensitivity) : Entity(position, rotation), camera(camera), sensitivity(sensitivity)
{
	setRotation(rotation);
}

Player::Player(glm::vec3 position, glm::vec3 rotation, const Camera& camera, float sensitivity) : Entity(position, rotation), camera(camera), sensitivity(sensitivity)
{
	setRotation(rotation);
}

void Player::setPosition(glm::vec3 position)
{
	Entity::setPosition(position);
	camera.setPosition(position);
}

glm::quat Player::getLookRotation()
{
	return camera.getRotation();
}

void Player::setRotation(glm::quat rotation)
{
	glm::vec3 euler = glm::eulerAngles(rotation);
	//this->rotation = glm::quat(glm::vec3(0, euler.y, 0));
	camera.setRotation(glm::quat(glm::vec3(euler.x, euler.y, 0)));
	updateVectors();
}

void Player::setRotation(glm::vec3 rotation)
{
	//this->rotation = glm::quat(glm::vec3(0.0f, glm::radians(rotation.y), 0.0f));
	//camera.setRotation(glm::quat(glm::vec3(glm::radians(rotation.x), 0.0f, 0.0f)));
	setRotation(glm::quat(glm::radians(rotation)));
}

void Player::update()
{
	const float cameraSpeed = 3 * DELTA_TIME;

	if (getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(GAME_WINDOW, true);
	}
	if (getKey(GLFW_KEY_W) == GLFW_PRESS) {
		move(glm::normalize(camera.getFront()) * cameraSpeed);
	}
	if (getKey(GLFW_KEY_S) == GLFW_PRESS) {
		move(glm::normalize(-camera.getFront()) * cameraSpeed);
	}
	if (getKey(GLFW_KEY_A) == GLFW_PRESS) {
		move(glm::normalize(-camera.getRight()) * cameraSpeed);
	}
	if (getKey(GLFW_KEY_D) == GLFW_PRESS) {
		move(glm::normalize(camera.getRight()) * cameraSpeed);
	}
	if (getKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
		move(glm::normalize(camera.getUp()) * cameraSpeed);
	}
}

void Player::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	yaw -= xOffset;
	pitch -= yOffset;
	pitch = std::max(std::min(pitch, 90.0f), -90.0f);
	camera.setRotation(glm::vec3(pitch, yaw, 0));
}
