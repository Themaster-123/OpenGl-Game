#include "physics_system.h"
#include "../../components/components.h"
#include "../renderer_system/renderer_system.h"
#include "../../physics.h"

glg::PhysicsSystem::PhysicsSystem() : ComponentSystem()
{
}

void glg::PhysicsSystem::physicsUpdate(Scene* scene)
{
	auto view = scene->registry.view<PhysicsComponent, TransformComponent>();

	for (auto entity : view) {
		Object obj(entity, scene);
		auto [physicsComponent, transformComponent] = obj.get<PhysicsComponent, TransformComponent>();
		rp3d::Transform physicsTransform = physicsComponent.collisionBody->getTransform();
		physicsComponent.prevTransform = transformComponent;
		transformComponent.position = physicsTransform.getPosition();
		transformComponent.rotation = physicsTransform.getOrientation();
	}
}

void glg::PhysicsSystem::registerDependencies(Scene* scene)
{
	Object::addConstruct<PhysicsComponent, PhysicsSystem>(scene);
	Object::addDestroy<PhysicsComponent, PhysicsSystem>(scene);
	Scene::DISPATCHER.sink<TransformSystem::onTransformUpdate>().connect<&PhysicsSystem::onTransformUpdate>();
}

void glg::PhysicsSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object object(entity, registry);
	TransformComponent& transformComponent = object.getOrAddComponent<TransformComponent>();
	PhysicsComponent& physicsComponent = object.get<PhysicsComponent>();
	physicsComponent.collisionBody->setTransform(transformComponent);
	physicsComponent.prevTransform = transformComponent;
}

void glg::PhysicsSystem::onDestroy(entt::registry& registry, entt::entity entity)
{
	Object object(entity, registry);
	auto& physicsComponent = object.get<PhysicsComponent>();
	
	rp3d::RigidBody* rigidbody = dynamic_cast<rp3d::RigidBody*>(physicsComponent.collisionBody);
	if (rigidbody != nullptr) {
		PHYSICS_WORLD->destroyRigidBody(rigidbody);
	}
	else {
		PHYSICS_WORLD->destroyCollisionBody(physicsComponent.collisionBody);
	}
}

void glg::PhysicsSystem::onTransformUpdate(const TransformSystem::onTransformUpdate& transformUpdate)
{
	if (transformUpdate.object.allOf<PhysicsComponent>()) {
		auto& physicsComponent = transformUpdate.object.get<PhysicsComponent>();
		physicsComponent.collisionBody->setTransform(transformUpdate.transform);
	}
}
