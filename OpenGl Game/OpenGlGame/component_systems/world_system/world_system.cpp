#include "world_system.h"

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
