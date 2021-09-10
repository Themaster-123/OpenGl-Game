#pragma once
#include "../../essential/object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace glg {
	struct TransformComponent
	{
	public:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;

		TransformComponent();

		TransformComponent(glm::vec3 position, glm::quat rotation);

		TransformComponent(glm::vec3 position, glm::vec3 rotation);

		virtual bool operator==(const TransformComponent& other) const;

		virtual TransformComponent& operator=(const TransformComponent& other);

	protected:

		virtual void updateVectors();
	};
}
