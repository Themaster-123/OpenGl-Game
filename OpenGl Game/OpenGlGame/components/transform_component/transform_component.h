#pragma once
#include "../components.h"
#include "../../essential/object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <entt/entt.hpp>

namespace glg {
	struct TransformComponent
	{
	public:
		TransformComponent() = default;

		TransformComponent(Object& object);

		TransformComponent(Object& object, glm::vec3 position, glm::quat rotation);

		TransformComponent(Object& object, glm::vec3 position, glm::vec3 rotation);

		virtual glm::vec3 getPosition() const;

		virtual void setPosition(glm::vec3 position);

		virtual glm::quat getRotation() const;

		virtual void setRotation(glm::quat rotation);

		virtual void setRotation(glm::vec3 rotation);

		virtual glm::vec3 getFront() const;

		virtual glm::vec3 getUp() const;

		virtual glm::vec3 getRight() const;

		virtual bool operator==(const TransformComponent& other) const;

		virtual TransformComponent& operator=(const TransformComponent& other);

	protected:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;

		virtual void updateVectors();
	};
}
