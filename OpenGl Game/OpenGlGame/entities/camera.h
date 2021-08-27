#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "entity.h"

namespace glg {
	class Camera : public Entity
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

		virtual glm::mat4 getViewMatrix();

		virtual glm::mat4 getProjectionMatrix();

	protected:
		glm::vec3 worldUp;
	};
}
