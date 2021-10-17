#include "world_system.h"

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

void glg::WorldSystem::loadChunk(const chunkVec& chunkPos, const WorldComponent& worldComponent)
{

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
