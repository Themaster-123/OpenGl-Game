#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace GLG {
	class Camera
	{
	public:
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio) : worldUp(0, 1, 0), fov(fov), aspectRatio(screenAspectRatio), nearPlane(0.01f), farPlane(100) {
			setPosition(position);
			setRotation(rotation);
			updateVectors();
		}

		Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane) : worldUp(0, 1, 0), fov(fov), aspectRatio(screenAspectRatio), nearPlane(nearPlane), farPlane(farPlane) {
			setPosition(position);
			setRotation(rotation);
			updateVectors();
		}

		Camera(glm::vec3 position, glm::vec3 rotation, float fov, float screenAspectRatio) : Camera(position, glm::quat(glm::radians(rotation)), fov, screenAspectRatio) {
		}

		Camera(glm::vec3 position, glm::vec3 rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane) : Camera(position, glm::quat(glm::radians(rotation)), fov, screenAspectRatio, nearPlane, farPlane) {
		}

		glm::mat4 getViewMatrix() {
			glm::mat4 view = glm::mat4(1);
			view = glm::translate(view, -position);
			view = glm::toMat4(glm::inverse(getRotation())) * view;
			return view;
		}

		glm::mat4 getProjectionMatrix() {
			return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		}

		glm::vec3 getPosition() {
			return position;
		}

		void setPosition(glm::vec3 position) {
			this->position = position;
		}

		glm::quat getRotation() {
			return rotation;
		}

		void setRotation(glm::quat rotation) {
			this->rotation = rotation;
			updateVectors();
		}

		void setRotation(glm::vec3 rotation) {
			this->rotation = glm::quat(glm::radians(rotation));
			updateVectors();
		}

		glm::vec3 getFront() {
			return front;
		}

		glm::vec3 getUp() {
			return up;
		}

		glm::vec3 getRight() {
			return right;
		}
	protected:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;

		void updateVectors() {
			front = glm::normalize(getRotation() * glm::vec3(0, 0, -1));
			right = glm::normalize(glm::cross(front, worldUp));
			up = glm::normalize(glm::cross(right, front));
		}
	};
}
