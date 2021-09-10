#pragma once
#include <entt/entt.hpp>
#include "../../components/components.h"
#include "../component_system.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


namespace glg {
	class TransformSystem : public ComponentSystem {
	public:
		TransformSystem();

		static void setPosition(Object& object, glm::vec3 position);

		static void setRotation(Object& object, glm::quat rotation);

		static void setRotation(Object& object, glm::vec3 rotation);

		static void setTransform(Object& object, TransformComponent& transform);

		static void updateVectors(Object& object);

		static glm::mat4 getModelMatrix(const TransformComponent& transform);

		static TransformComponent interpolateTransforms(const TransformComponent& prevTransform, const TransformComponent& currentTransform, float factor);

	protected:
	};
}