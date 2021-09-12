#include "renderer_system.h"
#include <iostream>
#include "../../scene.h"
#include "../../components/components.h"
#include "../transform_system/transform_system.h"
#include "../physics_system/physics_system.h"
#include "../../screen.h"

glg::RendererSystem::RendererSystem() : ComponentSystem()
{
	Object::addConstruct<ModelComponent, RendererSystem>();
	Object::addConstruct<CameraComponent, &RendererSystem::onCameraConstruct>();
}

void glg::RendererSystem::draw()
{
	auto modelView = scene::REGISTRY.view<ModelComponent, TransformComponent>(entt::exclude<PhysicsComponent>);

	for (auto entity : modelView) {
		Object obj(entity);
		drawModel(obj);
	}

	auto physicsModelView = scene::REGISTRY.view<ModelComponent, PhysicsComponent, TransformComponent>();

	for (auto entity : physicsModelView) {
		Object obj(entity);
		RendererSystem::drawPhysicsModel(obj);
	}
}

void glg::RendererSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<TransformComponent>();
}

void glg::RendererSystem::onCameraConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<TransformComponent>();
}

void glg::RendererSystem::drawModel(const Object& object)
{
	auto cameraView = scene::REGISTRY.view<CameraComponent, TransformComponent>();

	for (auto entity : cameraView) {
		Object cameraEntity(entity);

		const auto [modelComponent, transformComponent] = object.get<ModelComponent, TransformComponent>();
		
		modelComponent.shader.setMat4("view", getViewMatrix(cameraEntity));
		modelComponent.shader.setMat4("projection", getProjectionMatrix(cameraEntity));
		modelComponent.shader.setMat4("model", TransformSystem::getModelMatrix(transformComponent));
		modelComponent.model.draw(modelComponent.shader);
	}
}

void glg::RendererSystem::drawPhysicsModel(const Object& object)
{
	auto cameraView = scene::REGISTRY.view<CameraComponent, TransformComponent>();

	for (auto entity : cameraView) {
		Object cameraEntity(entity);

		const auto [modelComponent, transformComponent, physicsComponent] = object.get<ModelComponent, TransformComponent, PhysicsComponent>();

		modelComponent.shader.setMat4("view", getViewMatrix(cameraEntity));
		modelComponent.shader.setMat4("projection", getProjectionMatrix(cameraEntity));
		modelComponent.shader.setMat4("model", TransformSystem::getModelMatrix(TransformSystem::interpolateTransforms(physicsComponent.prevTransform, transformComponent, std::min(FACTOR, 1.0f))));
		modelComponent.model.draw(modelComponent.shader);
	}
}

glm::mat4 glg::RendererSystem::getViewMatrix(const Object& object)
{
	const auto [cameraComponent, transformComponent] = object.get<CameraComponent, TransformComponent>();

	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, -transformComponent.position);
	view = glm::toMat4(glm::inverse(transformComponent.rotation)) * view;
	return view;
}

glm::mat4 glg::RendererSystem::getProjectionMatrix(const Object& object)
{
	auto& cameraComponent = object.get<CameraComponent>();

	return glm::perspective(glm::radians(cameraComponent.fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, cameraComponent.nearPlane, cameraComponent.farPlane);
}
