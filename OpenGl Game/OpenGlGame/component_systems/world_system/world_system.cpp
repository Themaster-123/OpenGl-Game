#include "world_system.h"
#include "../../globals/textures.h"
#include "../../globals/shaders.h"
#include <thread>
#include <atomic>
#include "../../mesh_gen/marching_cubes.h"

glm::vec3 glg::CHUNK_SIZE = glm::vec3(32);

unsigned int glg::CHUNK_LOAD_SIZE = 6;

glm::ivec3 glg::CHUNK_RESOLUTION = glm::ivec3(32);

std::atomic<bool> CHUNK_LOAD_LOOP_RUNNING = true;

std::unordered_map<chunkVec, std::shared_ptr<glg::Model>, glg::ChunkPositionComparator> THREAD_CHUNK_MODELS;
std::mutex THREAD_CHUNK_MUTEX;

glg::WorldSystem::WorldSystem() : ComponentSystem()
{
	Object::addDestroy<WorldComponent, WorldSystem>();
	loadThread = std::thread(chunkLoadLoop);
}

glg::WorldSystem::~WorldSystem()
{
	CHUNK_LOAD_LOOP_RUNNING = false;
	loadThread.join();
}

void glg::WorldSystem::update()
{
	auto view = scene::REGISTRY.view<WorldComponent>();

	for (auto worldEntity : view) {
		Object world(worldEntity);

		auto& worldComponent = world.get<WorldComponent>();

		for (auto [pos, threadChunkModel] : THREAD_CHUNK_MODELS) {
			auto model = threadChunkModel;
			if (!isChunkLoaded(pos, worldComponent)) {
				//model->meshes[0].setupMesh();
				loadChunk(pos, model, worldComponent);
			}
			else {
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

			chunkVec chunkPos = getChunkPosition(transformComponent.position);

			std::vector<chunkVec> chunksToDelete;

			for (const auto& [pos, chunk] : worldComponent.chunks) {
				unsigned int distance = unsigned(getChunkDistance(chunkPos, pos));

				if (distance >= (CHUNK_LOAD_SIZE * 2) + 1) {
					chunksToDelete.push_back(pos);
				}
			}

			for (auto& pos : chunksToDelete) {
				unloadChunk(pos, worldComponent);
			}
		}
	}

	
}

void glg::WorldSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	Object object(entity);
	auto& worldComponent = object.get<WorldComponent>();

	for (auto [pos, chunkModelObject] : worldComponent.chunkModels) {
		chunkModelObject.destory();
	}
}

void glg::WorldSystem::loadChunk(const chunkVec& chunkPos, std::shared_ptr<Model> model, WorldComponent& worldComponent)
{
	if (!isChunkLoaded(chunkPos, worldComponent)) {
		worldComponent.chunksMutex.lock();
		auto extent = loadChunkModel(chunkPos, model, worldComponent);

		std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkPos, extent);
		worldComponent.chunks.insert(std::pair<chunkVec, std::shared_ptr<Chunk>>(chunkPos, chunk));

		worldComponent.chunksMutex.unlock();
	}
}

void glg::WorldSystem::unloadChunk(const chunkVec& chunkPos, WorldComponent& worldComponent)
{
	if (isChunkLoaded(chunkPos, worldComponent)) {
		worldComponent.chunksMutex.lock();
		unloadChunkModel(chunkPos, worldComponent);
		worldComponent.chunks.erase(chunkPos);
		worldComponent.chunksMutex.unlock();
	}
}

bool glg::WorldSystem::isChunkLoaded(const chunkVec& chunkPos, const WorldComponent& worldComponent)
{
	worldComponent.chunksMutex.lock();
	bool contains = worldComponent.chunks.contains(chunkPos);
	worldComponent.chunksMutex.unlock();
	return contains;
}

chunkVec glg::WorldSystem::getChunkPosition(const glm::vec3 position)
{
	chunkVec chunkPos;

	if (position.x < 0) {
		chunkPos.x = int(std::floor(position.x / CHUNK_SIZE.x));
	}
	else {
		chunkPos.x = int(position.x / CHUNK_SIZE.x);
	}

	if (position.y < 0) {
		chunkPos.y = int(std::floor(position.y / CHUNK_SIZE.y));
	}
	else {
		chunkPos.y = int(position.y / CHUNK_SIZE.y);
	}

	if (position.z < 0) {
		chunkPos.z = int(std::floor(position.z / CHUNK_SIZE.z));
	}
	else {
		chunkPos.z = int(position.z / CHUNK_SIZE.z);
	}

	return chunkPos;
}

int glg::WorldSystem::loadChunkModel(const chunkVec& chunkPos, std::shared_ptr<Model> model, WorldComponent& worldComponent)
{
	glm::ivec2 modelPos(chunkPos.x, chunkPos.z);

	if (!worldComponent.chunkModels.contains(modelPos)) {
		Object object;
		Model* cModel = new Model();
		cModel->meshes.emplace_back(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<Texture2D> { *textures::defaultTexture }, Material(), true, false);
		object.addComponent<TransformComponent>(glm::vec3(chunkPos.x * CHUNK_SIZE.x, 0, chunkPos.z * CHUNK_SIZE.z));
		object.addComponent<ModelComponent>(cModel, shaders::defaultShader);
		object.addComponent<ChunkStripComponent>();
		object.addComponent<BoxCullComponent>(glm::vec3(CHUNK_SIZE.x / 2, CHUNK_LOAD_SIZE * CHUNK_SIZE.y, CHUNK_SIZE.z / 2), glm::vec3(CHUNK_SIZE.x / 2, 0, CHUNK_SIZE.z / 2));


		worldComponent.chunkModels.insert(std::pair<const glm::ivec2, Object>(modelPos, object));
	}
	auto [modelComponent, chunkStripComponent, boxCullComponent] = worldComponent.chunkModels[modelPos].get<ModelComponent, ChunkStripComponent, BoxCullComponent>();

	Model* cModel = modelComponent.model;
	Mesh& cMesh = cModel->meshes[0];
	Mesh& mesh = model->meshes[0];

	cMesh.vertices.reserve(cMesh.vertices.size() + mesh.vertices.size());
	//cMesh.indices.reserve(cMesh.indices.size() + mesh.indices.size());

	int oldSize = cMesh.vertices.size();

	for (int i = 0; i < mesh.vertices.size(); i++) {
		cMesh.vertices.emplace_back(mesh.vertices[i].position + glm::vec3(0, chunkPos.y * CHUNK_SIZE.y, 0), mesh.vertices[i].normal, mesh.vertices[i].texCoords);
	}

	cMesh.setupMesh();

	chunkStripComponent.chunks.push_back(chunkPos);


	updateBoxCull(chunkStripComponent, boxCullComponent);
	return mesh.vertices.size();
}

void glg::WorldSystem::unloadChunkModel(const chunkVec& chunkPos, WorldComponent& worldComponent)
{
	glm::ivec2 modelPos(chunkPos.x, chunkPos.z);

	if (!worldComponent.chunkModels.contains(modelPos)) return;

	std::shared_ptr<Chunk> uChunk = worldComponent.chunks[chunkPos];

	auto [modelComponent, chunkStripComponent, boxCullComponent] = worldComponent.chunkModels[modelPos].get<ModelComponent, ChunkStripComponent, BoxCullComponent>();

	if (find(chunkStripComponent.chunks.begin(), chunkStripComponent.chunks.end(), chunkPos) == chunkStripComponent.chunks.end()) {
		return;
	}

	Model* model = modelComponent.model;
	Mesh& mesh = model->meshes[0];

	int index = chunkToIndex(chunkPos, worldComponent);

	//if (index > mesh.vertices.size() || index + uChunk->modelExtent > mesh.vertices.size()) {
	//	return;
	//}

	mesh.vertices.erase(mesh.vertices.begin() + index, mesh.vertices.begin() + index + uChunk->modelExtent);
	//chunkStripComponent.chunks.erase(std::remove(chunkStripComponent.chunks.begin(), chunkStripComponent.chunks.end(), chunkPos), chunkStripComponent.chunks.end());

	for (int i = 0; i < chunkStripComponent.chunks.size(); i++) {
		if (chunkPos == chunkStripComponent.chunks[i]) {
			chunkStripComponent.chunks.erase(chunkStripComponent.chunks.begin() + i);
			break;
		}
	}

	if (mesh.vertices.size() == 0) {
		worldComponent.chunkModels[modelPos].destory();
		delete model;
		worldComponent.chunkModels.erase(modelPos);
	}
	else {
		updateBoxCull(chunkStripComponent, boxCullComponent);
	}
}

void glg::WorldSystem::updateBoxCull(const ChunkStripComponent& stripChunkComp, BoxCullComponent& boxCullComp)
{
	int minY = std::numeric_limits<int>::max();
	int maxY = std::numeric_limits<int>::min();

	const auto& sChunks = stripChunkComp.chunks;

	for (int i = 0; i < sChunks.size(); i++) {
		const chunkVec& chunkPos = sChunks[i];

		if (chunkPos.y < minY) {
			minY = chunkPos.y;
		}

		if (chunkPos.y > maxY) {
			maxY = chunkPos.y;
		}
	}

	if (minY == std::numeric_limits<int>::max()) {
		minY = 0;
		maxY = 0;
	}

	float worldMinY = minY * CHUNK_SIZE.y;
	float worldMaxY = maxY * CHUNK_SIZE.y;

	boxCullComp.offset.y = (worldMinY + worldMaxY) / 2;
	boxCullComp.size.y = abs(worldMaxY - worldMinY) / 2;
}

int glg::WorldSystem::chunkToIndex(const chunkVec& chunkPos, const WorldComponent& worldComponent)
{
	glm::ivec2 modelPos(chunkPos.x, chunkPos.z);

	auto it = worldComponent.chunkModels.find(modelPos);

	if (it != worldComponent.chunkModels.end()) {
		int index = 0;

		const Object& object = it->second;

		const auto& chunkStripComponent = object.get<ChunkStripComponent>();
		const auto& sChunks = chunkStripComponent.chunks;

		auto stripChunkIt = find(sChunks.begin(), sChunks.end(), chunkPos);

		//if (stripChunkIt == sChunks.end()) {
		//	return -1;
		//}

		int stripIndex = std::distance(sChunks.begin(), stripChunkIt);

		for (int i = 0; i < stripIndex; i++) {
			auto chunkIt = worldComponent.chunks.find(sChunks[i]);

			if (chunkIt != worldComponent.chunks.end()) {
				std::shared_ptr<Chunk> chunk = chunkIt->second;
				index += chunk->modelExtent;
			}
		}

		return index;
	}

	return -1;
}

int glg::WorldSystem::getChunkDistance(const chunkVec& chunkPos1, const chunkVec& chunkPos2)
{
	chunkVec pos = chunkPos1 - chunkPos2;
	return std::max(std::max(abs(pos.x), abs(pos.y)), abs(pos.z));
}

void glg::WorldSystem::chunkLoadLoop()
{
	int realSize = (CHUNK_LOAD_SIZE * 2) + 1;
	int sizeSqrd = (realSize * realSize);

	// gets chunks ordered center to edges
	std::vector<chunkVec> chunksCTE = getClosestChunks(CHUNK_LOAD_SIZE);

	while (CHUNK_LOAD_LOOP_RUNNING) {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		scene::PLAYER_MUTEX.lock();

		for (auto entity : scene::PLAYERS) {
			Object object(entity);

			const auto& transformComponent = object.get<TransformComponent>();

			const auto& worldComponent = transformComponent.scene.get<WorldComponent>();

			chunkVec chunkPos = getChunkPosition(transformComponent.position);
			//chunkPos.y = 0;
			chunkVec loadPos;

			for (chunkVec offset : chunksCTE) {
				loadPos = chunkPos - offset;
				THREAD_CHUNK_MUTEX.lock();
				if (!THREAD_CHUNK_MODELS.contains(loadPos) && !isChunkLoaded(loadPos, worldComponent)) {
					THREAD_CHUNK_MUTEX.unlock();

					std::shared_ptr<Model> model = generateModel(loadPos, worldComponent);

					THREAD_CHUNK_MUTEX.lock();
					THREAD_CHUNK_MODELS.insert(std::pair<chunkVec, std::shared_ptr<Model>>(loadPos,
						model));
					THREAD_CHUNK_MUTEX.unlock();
					break;
				}
				else {
					THREAD_CHUNK_MUTEX.unlock();
				}
			}
		}

		scene::PLAYER_MUTEX.unlock();
	}
}

std::vector<chunkVec> glg::WorldSystem::getClosestChunks(uint32_t chunkSize)
{
	uint32_t realSize = (CHUNK_LOAD_SIZE * 2) + 1;
	uint32_t sizeSqrd = (realSize * realSize);
	uint32_t sizeCubed = (sizeSqrd * realSize);

	std::vector<chunkVec> traversedChunks;
	traversedChunks.reserve(sizeCubed);

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

std::shared_ptr<glg::Model> glg::WorldSystem::generateModel(const chunkVec& position, const WorldComponent& worldComponent)
{
	glm::ivec3 resolution = CHUNK_RESOLUTION + glm::ivec3(1);

	MarchingCubes cubes(resolution);

	float worldX, worldZ, worldY;
	std::vector<float> noiseOutput = worldComponent.noiseSettings.GenUniformNoise3D(glm::ivec3(position.x * CHUNK_RESOLUTION.x, position.y * CHUNK_RESOLUTION.y, position.z * CHUNK_RESOLUTION.z),
		glm::ivec3(resolution.x, resolution.y, resolution.z), .0050f, 2);

	for (int i = 0, x = 0; x < resolution.x; x++) {
		for (int z = 0; z < resolution.z; z++) {
			worldX = float(x) / CHUNK_RESOLUTION.x * CHUNK_SIZE.x;
			worldZ = float(z) / CHUNK_RESOLUTION.z * CHUNK_SIZE.z;
			//float noiseValue = world::NOISE_SETTINGS.noise.GetNoise(float(worldX + (position.x * world::CHUNK_SIZE.x)), float(worldZ + (position.z * world::CHUNK_SIZE.y)));
			//noiseValue = ((noiseValue + 1) / 2);

			//float displacementValue = noiseValue * world::NOISE_SETTINGS.displacementHeight / CHUNK_RESOLUTION.y * world::CHUNK_SIZE.y + 15;

			for (int y = 0; y < resolution.y; y++, i++) {
				worldY = float(y) / CHUNK_RESOLUTION.y * CHUNK_SIZE.y;

				//float noiseValue3D = world::NOISE_SETTINGS.noise->ge(float(worldX + (position.x * world::CHUNK_SIZE.x)), float(worldY) + (position.y * world::CHUNK_SIZE.y),
				//	float(worldZ + (position.z * world::CHUNK_SIZE.z)));
				////noiseValue3D = -((noiseValue3D + 1) / 2);
				//noiseValue3D *= world::NOISE_SETTINGS.displacementHeight;

				//float value = float(worldY) + (position.y * world::CHUNK_SIZE.y);

				//value += noiseValue3D;

				float value = noiseOutput[(z * resolution.x * resolution.y) + (y * resolution.x) + x];

				//if (noiseValue3D > .3)

				cubes.getVoxel(glm::ivec3(x, y, z)) = Voxel(glm::vec3(worldX, worldY, worldZ), value);
			}
		}
	}

	std::vector<Texture2D> textures{ *textures::defaultTexture };

	std::shared_ptr<Model> model = cubes.createModel(0.0f, textures);
	model->meshes[0].calculateNormals();

	return model;
}
