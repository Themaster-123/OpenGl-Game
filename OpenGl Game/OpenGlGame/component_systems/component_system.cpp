#include "component_system.h"
#include "../scene.h"
#include "renderer_system/renderer_system.h"
#include "transform_system/transform_system.h"
#include "physics_system/physics_system.h"
#include "player_system/player_system.h"
#include "chunk_loader_system/chunk_loader_system.h"


glg::ComponentSystem::ComponentSystem()
{
	scene::getSystems().push_back(this);
}

void glg::ComponentSystem::draw()
{
}

void glg::ComponentSystem::update()
{
}

void glg::ComponentSystem::physicsUpdate()
{
}

void glg::ComponentSystem::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos)
{
}

void glg::ComponentSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
}

void glg::ComponentSystem::addSystems()
{
	new RendererSystem();
	new TransformSystem();
	new PhysicsSystem();
	new PlayerSystem();
	new ChunkLoaderSystem();
}
