#include "player.h"
#include <utility>
#include "../../screen.h"
#include <iostream>

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

void Player::setRotation(glm::quat rotation)
{
	glm::vec3 euler = glm::eulerAngles(rotation);
	this->rotation = glm::quat(glm::vec3(0, euler.y, 0));
	camera.setRotation(glm::quat(glm::vec3(euler.x, euler.y, 0)));
	updateVectors();
}

void Player::setRotation(glm::vec3 rotation)
{
	camera.setRotation(glm::quat(glm::radians(rotation)));
}

void glg::Player::setLookRotation(glm::vec2 rotation)
{
	setRotation(glm::vec3(rotation.x, rotation.y, 0));
	lookRotation = rotation;
}

void Player::update()
{
	processInput();
}

void glg::Player::processInput()
{
	const float deltaSpeed = speed * DELTA_TIME;

	if (getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(GAME_WINDOW, true);
	}
	if (getKey(GLFW_KEY_W) == GLFW_PRESS) {
		move(glm::normalize(camera.getFront()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_S) == GLFW_PRESS) {
		move(glm::normalize(-camera.getFront()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_A) == GLFW_PRESS) {
		move(glm::normalize(-camera.getRight()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_D) == GLFW_PRESS) {
		move(glm::normalize(camera.getRight()) * deltaSpeed);
	}
	if (getKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
		move(glm::normalize(camera.getUp()) * deltaSpeed);
	}
}

void Player::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	glm::vec2 lookRot = getLookRotation();
	lookRot.y -= xOffset;
	lookRot.x -= yOffset;
	lookRot.x = std::max(std::min(lookRot.x, 90.0f), -90.0f);
	setLookRotation(lookRot);
}
