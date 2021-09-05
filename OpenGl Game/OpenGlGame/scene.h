#pragma once
#include "entities/entity.h"
#include "entities/Alive/player.h"
#include "entities/light/light_entity.h"

namespace glg {
	namespace scene {
		extern Player* MAIN_PLAYER;

		std::vector<LightEntity*>& getLights();

		std::vector<Entity*>& getEntities();

		void loopThroughEntitiesPhysics();
	}
}
