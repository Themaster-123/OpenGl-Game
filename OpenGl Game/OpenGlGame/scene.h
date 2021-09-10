#pragma once
#include "entities/entity.h"
#include "entities/Alive/player.h"
#include "entities/light/light_entity.h"
#include <entt/entt.hpp>
#include "component_systems/component_system.h"

namespace glg {
	namespace scene {
		extern Player* MAIN_PLAYER;

		extern entt::registry REGISTRY;

		extern entt::dispatcher DISPATCHER;

		std::vector<LightEntity*>& getLights();

		std::vector<Entity*>& getEntities();

		std::vector<ComponentSystem*>& getSystems();

		void loopThroughEntitiesPhysics();
	}
}
