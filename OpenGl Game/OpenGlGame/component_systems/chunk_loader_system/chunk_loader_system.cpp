#include "chunk_loader_system.h"
#include "../../components/components.h"
#include "../../screen.h"
#include "../../world/world.h"

float CHUNK_ACCUMULATOR = 0;

glg::ChunkLoaderSystem::ChunkLoaderSystem() : ComponentSystem()
{
}

void glg::ChunkLoaderSystem::update()
{
	CHUNK_ACCUMULATOR += DELTA_TIME;

	while (CHUNK_ACCUMULATOR >= world::CHUNK_LOAD_SPEED) {
		auto playerView = scene::REGISTRY.view<PlayerComponent, TransformComponent>();

		for (auto entity : playerView) {
			Object object(entity);

			auto [playerComponent, transformComponent] = playerView.get<PlayerComponent, TransformComponent>(entity);

			glm::ivec2 chunkPos = world::World::getChunkPosition(transformComponent.position);


			for (int x = -world::CHUNK_LOAD_SIZE; x <= world::CHUNK_LOAD_SIZE; x++) {
				for (int y = -world::CHUNK_LOAD_SIZE; y <= world::CHUNK_LOAD_SIZE; y++) {
					glm::ivec2 loadPos = chunkPos - glm::ivec2(x, y);

					if (!scene::WORLD.isChunkLoaded(loadPos)) {
						scene::WORLD.loadChunk(loadPos);
						goto chunkLoadOut;
					}
				}
			}

			chunkLoadOut:
			continue;

		}


		CHUNK_ACCUMULATOR -= world::CHUNK_LOAD_SPEED;
	}
}
