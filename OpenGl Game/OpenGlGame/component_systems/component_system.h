#pragma once
#include <entt/entt.hpp>

namespace glg {
	class ComponentSystem {
	public:
		ComponentSystem();

		virtual void draw();

		virtual void update();

		virtual void physicsUpdate();

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

		static void onConstruct(entt::registry& registry, entt::entity entity);

		static void addSystems();
	};
}
