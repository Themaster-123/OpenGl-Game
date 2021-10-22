#pragma once
#include <entt/entt.hpp>
#include "../scene.h"

namespace glg {
	class ComponentSystem {
	public:
		ComponentSystem();

		virtual ~ComponentSystem() = default;

		virtual void draw(Scene* scene);

		virtual void globalDraw();

		virtual void update(Scene* scene);

		virtual void physicsUpdate(Scene* scene);

		virtual void globalUpdate();

		virtual void globalPhysicsUpdate();

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos, Scene* scene);

		virtual void globalOnMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

		virtual void registerDependencies(Scene* scene);

		static void onConstruct(entt::registry& registry, entt::entity entity);

		static void addSystems();
	};
}
