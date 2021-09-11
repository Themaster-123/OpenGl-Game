#include "renderer_system.h"
#include <iostream>
#include "../../scene.h"
#include "../../components/components.h"
#include "../transform_system/transform_system.h"
#include "../physics_system/physics_system.h"

glg::RendererSystem::RendererSystem() : ComponentSystem()
{
	Object::addConstruct<ModelComponent, RendererSystem>();
}

void glg::RendererSystem::draw()
{
	auto modelView = scene::REGISTRY.view<ModelComponent>(entt::exclude<PhysicsComponent>);

	for (auto entity : modelView) {
		Object obj(entity);
		drawModel(obj);
	}

	auto physicsModelView = scene::REGISTRY.view<ModelComponent, PhysicsComponent>();

	for (auto entity : physicsModelView) {
		Object obj(entity);
		PhysicsSystem::drawModel(obj);
	}
}

void glg::RendererSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<TransformComponent>();
}

void glg::RendererSystem::drawModel(const Object& object)
{
	const auto [modelComponent, transformComponent] = object.get<ModelComponent, TransformComponent>();

	modelComponent.shader.setMat4("model", TransformSystem::getModelMatrix(transformComponent));
	modelComponent.model.draw(modelComponent.shader);
}
