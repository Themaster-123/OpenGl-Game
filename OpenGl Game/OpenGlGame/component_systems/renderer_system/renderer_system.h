#pragma once
#include "../component_system.h"
#include <entt/entt.hpp>

namespace glg {
	class RendererSystem : public ComponentSystem {
	public:
		RendererSystem();

		virtual void update();

		static void addDependencies(entt::registry& registry, entt::entity entity);
	};
}
