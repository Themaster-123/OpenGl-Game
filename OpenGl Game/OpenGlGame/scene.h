#pragma once
#include <entt/entt.hpp>
#include "component_systems/component_system.h"
#include <mutex>
#include <vector>

namespace glg {
	class Scene {
	public:
		static entt::dispatcher DISPATCHER;

		static std::vector<Scene*> SCENES;

		entt::registry registry;

		std::mutex PLAYER_MUTEX;

		Scene();

		~Scene();

		void clear();

		static std::vector<ComponentSystem*>& getGlobalSystems();

		static void callPhysicsUpdate();

	};
}
