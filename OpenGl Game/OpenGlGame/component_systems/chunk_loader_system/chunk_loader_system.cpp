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


			glm::ivec2 offsetPos(0, 0);
			glm::ivec2 direction(0, -1);
			glm::ivec2 loadPos;
			int sizeSqrd = ((world::CHUNK_LOAD_SIZE * 2) + 1) * ((world::CHUNK_LOAD_SIZE * 2) + 1);
			int moveAmount = 1;

			for (int i = 0, increase = 0, amountMoved = 0; i < sizeSqrd; i++, amountMoved++) {
				if (increase == 2) {
					moveAmount++;
					increase = 0;
				}

				loadPos = chunkPos - offsetPos;

				if (!scene::WORLD.isChunkLoaded(loadPos)) {
					scene::WORLD.loadChunk(loadPos);
					break;
				}

				if (amountMoved == moveAmount) {
					amountMoved = 0;
					int oldDirX = direction.x;
					direction.x = -direction.y;
					direction.y = oldDirX;
					increase++;
				}
				
				offsetPos += direction;
			}

			std::vector<glm::ivec2> chunksToDelete;

			for (const auto& [pos, chunk] : scene::WORLD.chunks) {
				glm::ivec2 localPos = chunkPos - pos;
				int distance = std::max(abs(localPos.x), abs(localPos.y));

				if (distance >= (world::CHUNK_LOAD_SIZE * 2) + 1) {
					chunksToDelete.push_back(pos);
				}
			}

			for (auto& pos : chunksToDelete) {
				scene::WORLD.unloadChunk(pos);
			}

		}

		CHUNK_ACCUMULATOR -= world::CHUNK_LOAD_SPEED;
	}
}
