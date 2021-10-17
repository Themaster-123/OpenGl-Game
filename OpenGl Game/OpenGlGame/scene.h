#pragma once
#include <entt/entt.hpp>
#include "component_systems/component_system.h"
#include <mutex>

namespace glg {

	namespace scene {

		extern entt::registry REGISTRY;

		extern entt::dispatcher DISPATCHER;

		extern std::mutex PLAYER_MUTEX;

		extern std::vector<entt::entity> PLAYERS;

		std::vector<ComponentSystem*>& getSystems();

		void callPhysicsUpdate();
	}
}
