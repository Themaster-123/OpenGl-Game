#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glg;

Camera::Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio) : Entity(position, rotation), worldUp(0, 1, 0), fov(fov), aspectRatio(screenAspectRatio), nearPlane(0.01f), farPlane(100) {
}

Camera::Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane) : Entity(position, rotation), worldUp(0, 1, 0), fov(fov), aspectRatio(screenAspectRatio), nearPlane(nearPlane), farPlane(farPlane) {
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