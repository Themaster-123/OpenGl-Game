#include "renderer_system.h"
#include <iostream>
#include "../../scene.h"
#include "../../components/components.h"

glg::RendererSystem::RendererSystem() : ComponentSystem()
{
	Object::addDependency<ModelComponent, RendererSystem>();
}

void glg::RendererSystem::update()
{
	auto view = scene::REGISTRY.view<ModelComponent>();

	for (auto entity : view) {
		Object obj(entity);
		auto& model = obj.get<ModelComponent>();

		model.draw();
	}
}

void glg::RendererSystem::addDependencies(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<TransformComponent>();
}
