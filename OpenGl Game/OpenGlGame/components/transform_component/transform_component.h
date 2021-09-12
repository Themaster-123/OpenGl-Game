#pragma once
#include "../../essential/object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <reactphysics3d/reactphysics3d.h>

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

		TransformComponent(const rp3d::Transform& transform);

		virtual bool operator==(const TransformComponent& other) const;

		virtual TransformComponent& operator=(const TransformComponent& other);

		operator rp3d::Transform() const;
	};
}
