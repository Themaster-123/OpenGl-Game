#include "chunk_loader_system.h"
#include "../../components/components.h"
#include "../../screen.h"
#include "../../world/world.h"
#include <reactphysics3d/reactphysics3d.h>
#include "../../physics.h"

float CHUNK_ACCUMULATOR = 0;

std::atomic<bool> CHUNK_LOAD_LOOP_RUNNING = true;

struct ThreadChunk {
	glg::Model* model;
	rp3d::TriangleVertexArray* triangleArray;
	rp3d::TriangleMesh* triangleMesh;
	rp3d::ConcaveMeshShape* concaveMesh;

	ThreadChunk(glg::Model* model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh) {
		this->model = model;
		this->triangleArray = triangleArray;
		this->triangleMesh = triangleMesh;
		this->concaveMesh = concaveMesh;
	}
};

std::unordered_map<glm::ivec2, ThreadChunk, glg::world::World::ChunkPositionComparator> THREAD_CHUNK_MODELS;
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
		threadChunk.model->meshes[0].setupMesh();
		scene::WORLD.loadChunk(pos, threadChunk.model, threadChunk.triangleArray, threadChunk.triangleMesh, threadChunk.concaveMesh);
	}
	THREAD_CHUNK_MUTEX.lock();
	THREAD_CHUNK_MODELS.clear();
	THREAD_CHUNK_MUTEX.unlock();

	auto playerView = scene::REGISTRY.view<PlayerComponent, TransformComponent>();

	for (auto entity : playerView) {
		Object object(entity);

		auto [playerComponent, transformComponent] = playerView.get<PlayerComponent, TransformComponent>(entity);

		glm::ivec2 chunkPos = world::World::getChunkPosition(transformComponent.position);

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
}

void glg::ChunkLoaderSystem::chunkLoadLoop()
{
	while (CHUNK_LOAD_LOOP_RUNNING) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		scene::PLAYER_MUTEX.lock();

		for (auto entity : scene::PLAYERS) {
			Object object(entity);

			const auto& transformComponent = object.get<TransformComponent>();

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
					Model* model = glg::world::Chunk::generateModel(loadPos);
					Mesh& mesh = model->meshes[0];

					rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(mesh.vertices.size(), &mesh.vertices[0], sizeof(Vertex), mesh.indices.size() / 3, &mesh.indices[0], 3 * sizeof(unsigned int),
						rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
						rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
					rp3d::TriangleMesh* triangleMesh = PHYSICS_COMMON.createTriangleMesh();
					triangleMesh->addSubpart(triangleArray);
					rp3d::ConcaveMeshShape* concaveMesh = PHYSICS_COMMON.createConcaveMeshShape(triangleMesh);

					THREAD_CHUNK_MUTEX.lock();
					THREAD_CHUNK_MODELS.insert(std::pair<glm::ivec2, ThreadChunk>(loadPos, ThreadChunk(model, triangleArray, triangleMesh, concaveMesh)));
					THREAD_CHUNK_MUTEX.unlock();
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
		}

		scene::PLAYER_MUTEX.unlock();
	}
}
