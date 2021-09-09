#include "component_system.h"
#include "../scene.h"
#include "renderer_system/renderer_system.h"

glg::ComponentSystem::ComponentSystem()
{
	scene::getSystems().push_back(this);
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

void glg::ComponentSystem::addDependencies(entt::registry& registry, entt::entity entity)
{
}

void glg::ComponentSystem::addSystems()
{
	new RendererSystem();
}
