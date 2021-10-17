#include "chunk_loader_system.h"
#include "../../components/components.h"
#include "../../screen.h"
#include "../../world/world.h"
#include <reactphysics3d/reactphysics3d.h>
#include "../../physics.h"



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
			//model->meshes[0].setupMesh();
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

}
}
