#include "physics_system.h"
#include "../../components/components.h"
#include "../renderer_system/renderer_system.h"

glg::PhysicsSystem::PhysicsSystem() : ComponentSystem()
{
	Object::addConstruct<PhysicsComponent, PhysicsSystem>();
	scene::DISPATCHER.sink<TransformSystem::onTransformUpdate>().connect<&PhysicsSystem::onTransformUpdate>();
}

void glg::PhysicsSystem::physicsUpdate()
{
	auto view = scene::REGISTRY.view<PhysicsComponent>();

	for (auto entity : view) {
		Object obj(entity);
		auto [physicsComponent, transformComponent] = obj.get<PhysicsComponent, TransformComponent>();
		rp3d::Transform physicsTransform = physicsComponent.collisionBody->getTransform();
		physicsComponent.prevTransform = transformComponent;
		transformComponent.position = physicsTransform.getPosition();
		transformComponent.rotation = physicsTransform.getOrientation();
	}
}

void glg::PhysicsSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object object(entity);
	object.getOrAddComponent<TransformComponent>();
}

void glg::PhysicsSystem::drawModel(const Object& object)
{
	const auto [modelComponent, transformComponent, physicsComponent] = object.get<ModelComponent, TransformComponent, PhysicsComponent>();

	modelComponent.shader.setMat4("model", TransformSystem::getModelMatrix(TransformSystem::interpolateTransforms(physicsComponent.prevTransform, transformComponent, std::min(FACTOR, 1.0f))));
	modelComponent.model.draw(modelComponent.shader);
}

void glg::PhysicsSystem::onTransformUpdate(const TransformSystem::onTransformUpdate& transformUpdate)
{
	auto& physicsComponent = transformUpdate.object.get<PhysicsComponent>();
	physicsComponent.collisionBody->setTransform(transformUpdate.transform);
}
