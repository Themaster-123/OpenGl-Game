#pragma once
#include <entt/entt.hpp>
#include "component_systems/component_system.h"

namespace glg {
	namespace scene {

		extern entt::registry REGISTRY;

		extern entt::dispatcher DISPATCHER;

		std::vector<ComponentSystem*>& getSystems();

		void callPhysicsUpdate();
	}
}
