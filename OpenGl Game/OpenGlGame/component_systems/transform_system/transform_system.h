#pragma once
#include <entt/entt.hpp>
#include "../../components/components.h"
#include "../component_system.h"

namespace glg {
	class TransformSystem : public ComponentSystem {
	public:
		TransformSystem();

		static void setPosition(Object& object, glm::vec3 position);

		static void setRotation(Object& object, glm::quat rotation);

		static void setRotation(Object& object, glm::vec3 rotation);

		static void setTransform(Object& object, TransformComponent& transform);

		static void updateVectors(Object& object);

	protected:
	};
}