#include "renderer_system.h"
#include <iostream>
#include "../../scene.h"
#include "../../components/components.h"

glg::RendererSystem::RendererSystem() : ComponentSystem()
{
	Object::addConstruct<ModelComponent, RendererSystem>();
}

void glg::RendererSystem::draw()
{
	auto view = scene::REGISTRY.view<ModelComponent>();

	for (auto entity : view) {
		Object obj(entity);
		auto& model = obj.get<ModelComponent>();

		model.draw(obj.get<TransformComponent>().getModelMatrix());
	}
}

void glg::RendererSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<TransformComponent>();
}
