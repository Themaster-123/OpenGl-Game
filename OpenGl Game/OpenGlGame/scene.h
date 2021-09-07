#pragma once
#include "entities/entity.h"
#include "entities/Alive/player.h"
#include "entities/light/light_entity.h"
#include <entt/entt.hpp>

namespace glg {
	namespace scene {
		extern Player* MAIN_PLAYER;
		extern entt::registry REGISTRY;

		std::vector<LightEntity*>& getLights();

		std::vector<Entity*>& getEntities();

		void loopThroughEntitiesPhysics();
	}
}
