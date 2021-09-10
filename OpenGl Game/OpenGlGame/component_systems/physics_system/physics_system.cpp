#include "physics_system.h"
#include "../../components/components.h"

glg::PhysicsSystem::PhysicsSystem() : ComponentSystem()
{
}

void glg::PhysicsSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object object(entity);

}
