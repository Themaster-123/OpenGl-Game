#pragma once
#include "../component_system.h"

namespace glg {
	class PhysicsSystem : public ComponentSystem
	{
	public:
		PhysicsSystem();

		virtual void physicsUpdate();

		static void onConstruct(entt::registry& registry, entt::entity entity);
	};
}

