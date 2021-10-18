#pragma once
#include <entt/entt.hpp>
#include "component_systems/component_system.h"
#include <mutex>

namespace glg {

	namespace scene {

		std::vector<ComponentSystem*>& getGlobalSystems();

		class Scene {
		public:
			static std::vector<Scene> SCENES;

			entt::registry registry;

			std::mutex PLAYER_MUTEX;

			Scene();

			void callPhysicsUpdate();

			std::vector<ComponentSystem*>& getSystems();

		};
	}
}
