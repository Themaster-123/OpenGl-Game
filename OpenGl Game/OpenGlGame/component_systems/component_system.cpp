#include "component_system.h"
#include "../scene.h"
#include "renderer_system/renderer_system.h"
#include "transform_system/transform_system.h"
#include "physics_system/physics_system.h"
#include "player_system/player_system.h"
#include "world_system/world_system.h"


glg::ComponentSystem::ComponentSystem()
{
	scene::getGlobalSystems().push_back(this);
}

void glg::ComponentSystem::draw(scene::Scene& scene)
{
}

void glg::ComponentSystem::globalDraw()
{
}

void glg::ComponentSystem::update(scene::Scene& scene)
{
}

void glg::ComponentSystem::physicsUpdate(scene::Scene& scene)
{
}

void glg::ComponentSystem::globalUpdate()
{
}

void glg::ComponentSystem::globalPhysicsUpdate()
{
}

void glg::ComponentSystem::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos, scene::Scene& scene)
{
}

void glg::ComponentSystem::globalOnMouseMovement(float xOffset, float yOffset, float xPos, float yPos)
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
	new WorldSystem();
}
