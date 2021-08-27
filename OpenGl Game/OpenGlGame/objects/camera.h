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

		Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio);

		Camera(glm::vec3 position, glm::quat rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane);

		Camera(glm::vec3 position, glm::vec3 rotation, float fov, float screenAspectRatio);

		Camera(glm::vec3 position, glm::vec3 rotation, float fov, float screenAspectRatio, float nearPlane, float farPlane);

		glm::mat4 getViewMatrix();

		glm::mat4 getProjectionMatrix();

		glm::vec3 getPosition();

		void setPosition(glm::vec3 position);

		glm::quat getRotation();

		void setRotation(glm::quat rotation);

		void setRotation(glm::vec3 rotation);

		glm::vec3 getFront();

		glm::vec3 getUp();

		glm::vec3 getRight();
	protected:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;

		void updateVectors();
	};
}
