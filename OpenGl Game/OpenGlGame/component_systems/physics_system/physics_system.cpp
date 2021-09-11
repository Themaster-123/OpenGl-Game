#include "physics_system.h"
#include "../../components/components.h"

glg::PhysicsSystem::PhysicsSystem() : ComponentSystem()
{
	Object::addConstruct<PhysicsComponent, PhysicsSystem>();
}

void glg::PhysicsSystem::physicsUpdate()
{
	auto view = scene::REGISTRY.view<PhysicsComponent>();

	for (auto entity : view) {
		Object obj(entity);
		auto [physicsComponent, transformComponent] = obj.get<PhysicsComponent, TransformComponent>();
		rp3d::Transform physicsTransform = physicsComponent.collisionBody->getTransform();
		transformComponent.position = physicsTransform.getPosition();
		transformComponent.rotation = physicsTransform.getOrientation();
	}
}

void glg::PhysicsSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object object(entity);
	object.getOrAddComponent<TransformComponent>();
}
