#pragma once
#include <entt/entt.hpp>
#include "component_systems/component_system.h"
#include <mutex>

namespace glg {
	namespace world {
		class World;
	}

	namespace scene {

		extern entt::registry REGISTRY;

		extern entt::dispatcher DISPATCHER;

		extern world::World WORLD;

		extern std::mutex PLAYER_MUTEX;

		extern std::vector<entt::entity> PLAYERS;

		std::vector<ComponentSystem*>& getSystems();

		void callPhysicsUpdate();
	}
}
