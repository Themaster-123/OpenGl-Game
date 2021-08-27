#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace GLG;

Camera::Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio) : worldUp(0, 1, 0), fov(fov), aspectRatio(screenAspectRatio), nearPlane(0.01f), farPlane(100) {
	setPosition(position);
	setRotation(rotation);
	updateVectors();
}

Camera::Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane) : worldUp(0, 1, 0), fov(fov), aspectRatio(screenAspectRatio), nearPlane(nearPlane), farPlane(farPlane) {
	setPosition(position);
	setRotation(rotation);
	updateVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float screenAspectRatio) : Camera(position, glm::quat(glm::radians(rotation)), fov, screenAspectRatio) {
}

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane) : Camera(position, glm::quat(glm::radians(rotation)), fov, screenAspectRatio, nearPlane, farPlane) {
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, -position);
	view = glm::toMat4(glm::inverse(getRotation())) * view;
	return view;
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getPosition() {
	return position;
}

void Camera::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::quat Camera::getRotation() {
	return rotation;
}

void Camera::setRotation(glm::quat rotation) {
	this->rotation = rotation;
	updateVectors();
}

void Camera::setRotation(glm::vec3 rotation) {
	this->rotation = glm::quat(glm::radians(rotation));
	updateVectors();
}

glm::vec3 Camera::getFront() {
	return front;
}

glm::vec3 Camera::getUp() {
	return up;
}

glm::vec3 Camera::getRight() {
	return right;
}

void Camera::updateVectors() {
	front = glm::normalize(getRotation() * glm::vec3(0, 0, -1));
	right = glm::normalize(getRotation() * glm::vec3(1, 0, 0));
	up = glm::normalize(getRotation() * glm::vec3(0, 1, 0));
}