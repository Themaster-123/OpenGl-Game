#pragma once
#include "../component_system.h"
#include <entt/entt.hpp>

namespace glg {
	class RendererSystem : public ComponentSystem {
	public:
		RendererSystem();

		virtual void draw();

		static void onConstruct(entt::registry& registry, entt::entity entity);
	};
}
