#include "chunk_loader_system.h"
#include "../../components/components.h"
#include "../../screen.h"
#include "../../world/world.h"
#include <reactphysics3d/reactphysics3d.h>
#include "../../physics.h"

std::atomic<bool> CHUNK_LOAD_LOOP_RUNNING = true;

std::unordered_map<chunkVec, std::tuple<std::shared_ptr<glg::Model>, rp3d::TriangleVertexArray*, rp3d::TriangleMesh*, rp3d::ConcaveMeshShape*>, glg::world::World::ChunkPositionComparator> THREAD_CHUNK_MODELS;
std::mutex THREAD_CHUNK_MUTEX;

glg::ChunkLoaderSystem::ChunkLoaderSystem() : ComponentSystem()
{
	loadThread = std::thread(chunkLoadLoop);
}

glg::ChunkLoaderSystem::~ChunkLoaderSystem()
{
	CHUNK_LOAD_LOOP_RUNNING = false;
	loadThread.join();
}

void glg::ChunkLoaderSystem::update()
{

	for (auto [pos, threadChunk] : THREAD_CHUNK_MODELS) {
		auto [model, triangleArray, triangleMesh, concaveMesh] = threadChunk;
		if (!scene::WORLD.isChunkLoaded(pos)) {
			model->meshes[0].setupMesh();
			scene::WORLD.loadChunk(pos, model, triangleArray, triangleMesh, concaveMesh);
		}
		else {
			delete triangleArray;
			//PHYSICS_COMMON.destroyTriangleMesh(triangleMesh);
			//PHYSICS_COMMON.destroyConcaveMeshShape(concaveMesh);
		}
	}
	THREAD_CHUNK_MUTEX.lock();
	THREAD_CHUNK_MODELS.clear();
	THREAD_CHUNK_MUTEX.unlock();

	auto playerView = scene::REGISTRY.view<PlayerComponent, TransformComponent>();

	for (auto entity : playerView) {
		Object object(entity);

		const auto& transformComponent = playerView.get<TransformComponent>(entity);

		chunkVec chunkPos = world::World::getChunkPosition(transformComponent.position);

		std::vector<chunkVec> chunksToDelete;

		for (const auto& [pos, chunk] : scene::WORLD.chunks) {
			unsigned int distance = unsigned(world::World::getChunkDistance(chunkPos, pos));

			if (distance >= (world::CHUNK_LOAD_SIZE * 2) + 1) {
				chunksToDelete.push_back(pos);
			}
		}

		for (auto& pos : chunksToDelete) {
			scene::WORLD.unloadChunk(pos);
		}
	}
}

void glg::ChunkLoaderSystem::chunkLoadLoop()
{
	int realSize = (world::CHUNK_LOAD_SIZE * 2) + 1;
	int sizeSqrd = (realSize * realSize);

	// gets chunks ordered center to edges
	std::vector<chunkVec> chunksCTE = getClosestChunks(world::CHUNK_LOAD_SIZE);

	while (CHUNK_LOAD_LOOP_RUNNING) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		scene::PLAYER_MUTEX.lock();

		for (auto entity : scene::PLAYERS) {
			Object object(entity);

			const auto& transformComponent = object.get<TransformComponent>();

			chunkVec chunkPos = world::World::getChunkPosition(transformComponent.position);
			//chunkPos.y = 0;
			chunkVec loadPos;

			for (chunkVec offset : chunksCTE) {
				loadPos = chunkPos - offset;
				THREAD_CHUNK_MUTEX.lock();
				if (!THREAD_CHUNK_MODELS.contains(loadPos) && !scene::WORLD.isChunkLoaded(loadPos)) {
					THREAD_CHUNK_MUTEX.unlock();

					std::shared_ptr<Model> model = glg::world::Chunk::generateModel(loadPos);

					auto [triangleArray, triangleMesh, concaveMesh] = glg::world::Chunk::generateConcaveMeshShape(model);

					THREAD_CHUNK_MUTEX.lock();
					THREAD_CHUNK_MODELS.insert(std::pair<chunkVec, std::tuple<std::shared_ptr<Model>, rp3d::TriangleVertexArray*, rp3d::TriangleMesh*, rp3d::ConcaveMeshShape*>>(loadPos,
						{ model, triangleArray, triangleMesh, concaveMesh }));
					THREAD_CHUNK_MUTEX.unlock();
					break;
				}
				else {
					THREAD_CHUNK_MUTEX.unlock();
				}
			}

			/*chunkVec offsetPos(0, 0, 0);
			glm::ivec2 direction(0, -1);
			int moveAmount = 1;

			for (int i = 0, increase = 0, amountMoved = 0; i < sizeSqrd; i++, amountMoved++) {
				if (increase == 2) {
					moveAmount++;
					increase = 0;
				}

				loadPos = chunkPos - offsetPos;

				THREAD_CHUNK_MUTEX.lock();
				if (!THREAD_CHUNK_MODELS.contains(loadPos) && !scene::WORLD.isChunkLoaded(loadPos)) {
					THREAD_CHUNK_MUTEX.unlock();

					std::shared_ptr<Model> model = glg::world::Chunk::generateModel(loadPos);

					auto [triangleArray, triangleMesh, concaveMesh] = glg::world::Chunk::generateConcaveMeshShape(model);

					THREAD_CHUNK_MUTEX.lock();
					THREAD_CHUNK_MODELS.insert(std::pair<chunkVec, std::tuple<std::shared_ptr<Model>, rp3d::TriangleVertexArray*, rp3d::TriangleMesh*, rp3d::ConcaveMeshShape*>>(loadPos,
						{ model, triangleArray, triangleMesh, concaveMesh }));
					THREAD_CHUNK_MUTEX.unlock();
					break;
				}
				else {
					THREAD_CHUNK_MUTEX.unlock();
				}

				if (amountMoved == moveAmount) {
					amountMoved = 0;
					int oldDirX = direction.x;
					direction.x = -direction.y;
					direction.y = oldDirX;
					increase++;
				}

				offsetPos.x += direction.x;
				offsetPos.z += direction.y;
			}*/
		}

		scene::PLAYER_MUTEX.unlock();
	}
}

std::vector<chunkVec> glg::ChunkLoaderSystem::getClosestChunks(uint32_t chunkSize)
{
	uint32_t realSize = (world::CHUNK_LOAD_SIZE * 2) + 1;
	uint32_t sizeSqrd = (realSize * realSize);
	uint32_t sizeCubed = (sizeSqrd * realSize);

	std::vector<chunkVec> traversedChunks;
	traversedChunks.reserve(sizeCubed);

	/*for (auto z = 0u; z <= 3 * chunkSize; z++) {
		for (auto x = std::max(0u, z - 2 * chunkSize); x <= std::min(chunkSize, z); x++) {
			for (auto y = std::max(0u, z - x - chunkSize); y <= std::min(chunkSize, z - x); y++) {
				chunkVec pos(x, y, z - x - y);

				traversedChunks.emplace_back(chunkSize + pos.x, chunkSize + pos.y, chunkSize + pos.z);
				if (pos.z) traversedChunks.emplace_back(chunkSize + pos.x, chunkSize + pos.y, chunkSize - pos.z);
				if (pos.y) traversedChunks.emplace_back(chunkSize + pos.x, chunkSize - pos.y, chunkSize + pos.z);
				if (pos.y && pos.z) traversedChunks.emplace_back(chunkSize + pos.x, chunkSize - pos.y, chunkSize - pos.z);
				if (pos.x) traversedChunks.emplace_back(chunkSize - pos.x, chunkSize + pos.y, chunkSize + pos.z);
				if (pos.x && pos.z) traversedChunks.emplace_back(chunkSize - pos.x, chunkSize + pos.y, chunkSize - pos.z);
				if (pos.x && pos.y) traversedChunks.emplace_back(chunkSize - pos.x, chunkSize - pos.y, chunkSize + pos.z);
				if (pos.x && pos.y && pos.z) traversedChunks.emplace_back(chunkSize - pos.x, chunkSize - pos.y, chunkSize - pos.z);
			}
		}
	}*/
	chunkVec offsetPos(0, 0, 0);
	glm::ivec2 direction(0, -1);
	int moveAmount = 1;

	for (int i = 0, increase = 0, amountMoved = 0; i < sizeSqrd; i++, amountMoved++) {
		if (increase == 2) {
			moveAmount++;
			increase = 0;
		}

		traversedChunks.push_back(offsetPos);

		for (int i = 1; i <= chunkSize; i++) {
			traversedChunks.push_back(offsetPos + chunkVec(0, -i, 0));
		}
		for (int i = 1; i <= chunkSize; i++) {
			traversedChunks.push_back(offsetPos + chunkVec(0, i, 0));
		}

		if (amountMoved == moveAmount) {
			amountMoved = 0;
			int oldDirX = direction.x;
			direction.x = -direction.y;
			direction.y = oldDirX;
			increase++;
		}

		offsetPos.x += direction.x;
		offsetPos.z += direction.y;
	}

	return traversedChunks;
}
