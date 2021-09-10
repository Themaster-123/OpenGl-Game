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

		virtual glm::vec3 getPosition() const;

		virtual void setPosition(glm::vec3 position);

		virtual glm::quat getRotation() const;

		virtual void setRotation(glm::quat rotation);

		virtual void setRotation(glm::vec3 rotation);

		virtual glm::vec3 getFront() const;

		virtual glm::vec3 getUp() const;

		virtual glm::vec3 getRight() const;

		virtual glm::mat4 getModelMatrix() const;

		virtual bool operator==(const TransformComponent& other) const;

		virtual TransformComponent& operator=(const TransformComponent& other);

		static TransformComponent interpolateTransforms(const TransformComponent& prevTransform, const TransformComponent& currentTransform, float factor);

	protected:

		virtual void updateVectors();
	};
}
