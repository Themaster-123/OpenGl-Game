#pragma once
#include "../component_system.h"
#include "../../globals/macros.h"
#include <glm/glm.hpp>
#include "../../components/components.h"

namespace glg {
	class WorldSystem : public ComponentSystem {
	public:
		WorldSystem();

		static void loadChunk(const chunkVec& chunkPos, const WorldComponent& worldComponent);
	};
}
