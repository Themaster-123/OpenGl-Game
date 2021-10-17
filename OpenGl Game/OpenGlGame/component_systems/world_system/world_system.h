#pragma once
#include "../component_system.h"
#include "../../globals/macros.h"
#include <glm/glm.hpp>
#include "../../components/components.h"
#include <memory>

namespace glg {
	extern unsigned int CHUNK_LOAD_SIZE;

	extern glm::ivec3 CHUNK_RESOLUTION;

	extern glm::vec3 CHUNK_SIZE;

	class WorldSystem : public ComponentSystem {
	public:
		std::thread loadThread;

		WorldSystem();

		~WorldSystem();

		void update();

		static void onDestroy(entt::registry& registry, entt::entity entity);

		static void loadChunk(const chunkVec& chunkPos, std::shared_ptr<Model> model, WorldComponent& worldComponent);

		void unloadChunk(const chunkVec& chunkPos, WorldComponent& worldComponent);

		static bool isChunkLoaded(const chunkVec& chunkPos, const WorldComponent& worldComponent);

		static chunkVec getChunkPosition(const glm::vec3 position);

		static int loadChunkModel(const chunkVec& chunkPos, std::shared_ptr<Model> model, WorldComponent& worldComponent);
		
		static void unloadChunkModel(const chunkVec& chunkPos, WorldComponent& worldComponent);

		static void updateBoxCull(const ChunkStripComponent& stripChunkComp, BoxCullComponent& boxCullComp);

		static int chunkToIndex(const chunkVec& chunkPos, const WorldComponent& worldComponent);

		static int getChunkDistance(const chunkVec& chunkPos1, const chunkVec& chunkPos2);

		static void chunkLoadLoop();

		static std::vector<chunkVec> getClosestChunks(uint32_t chunkSize);

		static std::shared_ptr<Model> generateModel(const chunkVec& position, const WorldComponent& worldComponent);
	};
}
