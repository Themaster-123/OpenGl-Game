#pragma once
#include "../component_system.h"
#include "../../globals/macros.h"
#include <glm/glm.hpp>
#include "../../components/components.h"

namespace glg {
	extern unsigned int CHUNK_LOAD_SIZE;

	extern glm::ivec3 CHUNK_RESOLUTION;

	extern glm::vec3 CHUNK_SIZE;

	class WorldSystem : public ComponentSystem {
	public:
		WorldSystem();

		static void onDestroy(entt::registry& registry, entt::entity entity);

		static void loadChunk(const chunkVec& chunkPos, const WorldComponent& worldComponent);

		static bool isChunkLoaded(const chunkVec& chunkPos, const WorldComponent& worldComponent);

		static chunkVec getChunkPosition(const glm::vec3 position);
	};
}
