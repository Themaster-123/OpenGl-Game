#include "world_system.h"
#include "../../globals/textures.h"
#include "../../globals/shaders.h"

glm::vec3 glg::CHUNK_SIZE = glm::vec3(32);

unsigned int glg::CHUNK_LOAD_SIZE = 6;

glm::ivec3 glg::CHUNK_RESOLUTION = glm::ivec3(32);

glg::WorldSystem::WorldSystem()
{
	Object::addDestroy<WorldComponent, WorldSystem>();
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

	int index = chunkToIndex(chunkPos);

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
