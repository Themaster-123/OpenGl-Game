#include "renderer_system.h"
#include <iostream>
#include "../../scene.h"
#include "../../components/components.h"
#include "../transform_system/transform_system.h"
#include "../physics_system/physics_system.h"
#include "../../screen.h"
#include "../../physics.h"
#include "../../globals/shaders.h"

glg::RendererSystem::RendererSystem() : ComponentSystem()
{
	Object::addConstruct<ModelComponent, RendererSystem>();
	Object::addConstruct<CameraComponent, &RendererSystem::onCameraConstruct>();
	Object::addConstruct<LightComponent, RendererSystem>();
	Object::addConstruct<DirectionalLightComponent, &RendererSystem::onLightConstruct>();
	Object::addConstruct<AttenuationLightComponent, &RendererSystem::onLightConstruct>();
	Object::addConstruct<PointLightComponent, &RendererSystem::onAttenuationLightConstruct>();
	Object::addConstruct<SpotLightComponent, &RendererSystem::onAttenuationLightConstruct>();
}

void glg::RendererSystem::draw()
{
	scene::REGISTRY.sort<LightComponent>([](const auto& light1, const auto& light2) {
		return light1.priority > light2.priority;
	});

	auto lightView = scene::REGISTRY.view<LightComponent>();

	int index = 0;
	for (auto entity : lightView) {
		Object obj(entity);

		auto& lightComponent = obj.get<LightComponent>(lightView);
		auto& transformComponent = obj.get<TransformComponent>();

		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant = 0;
		float linear = 0;
		float quadratic = 0;
		float innerCutOff = 0;
		float outerCutOff = 0;

		position = transformComponent.position;
		direction = transformComponent.front;
		ambient = lightComponent.ambient;
		diffuse = lightComponent.diffuse;
		specular = lightComponent.specular;

		if (obj.allOf<DirectionalLightComponent>()) {
			RendererSystem::setLightUniforms(position, direction, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff, 0, index, lightView.size());
		}
		else if (obj.allOf<AttenuationLightComponent>()) {
			AttenuationLightComponent& attenuationLightComponent = obj.get<AttenuationLightComponent>();
			constant = attenuationLightComponent.constant;
			linear = attenuationLightComponent.linear;
			quadratic = attenuationLightComponent.quadratic;

			if (obj.allOf<PointLightComponent>()) {
				RendererSystem::setLightUniforms(position, direction, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff, 1, index, lightView.size());
			}
			else if (obj.allOf<SpotLightComponent>()) {
				SpotLightComponent& spotLightComponent = obj.get<SpotLightComponent>();

				innerCutOff = spotLightComponent.innerCutOff;
				outerCutOff = spotLightComponent.outerCutOff;

				RendererSystem::setLightUniforms(position, direction, ambient, diffuse, specular, constant, linear, quadratic, innerCutOff, outerCutOff, 2, index, lightView.size());
			}
		}

		index++;
	}

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

void glg::RendererSystem::onLightConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<LightComponent>();
}

void glg::RendererSystem::onAttenuationLightConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<AttenuationLightComponent>();
}

void glg::RendererSystem::drawModel(const Object& object)
{
	const auto& transformComponent = object.get<TransformComponent>();

	drawModel(object, transformComponent);
}

void glg::RendererSystem::drawPhysicsModel(const Object& object)
{
	const auto [transformComponent, physicsComponent] = object.get<TransformComponent, PhysicsComponent>();

	auto cameraView = scene::REGISTRY.view<CameraComponent, TransformComponent>();

	drawModel(object, TransformSystem::interpolateTransforms(physicsComponent.prevTransform, transformComponent, std::min(FACTOR, 1.0f)));
}

void glg::RendererSystem::drawModel(const Object& object, const TransformComponent& transformComponent)
{
	auto cameraView = scene::REGISTRY.view<CameraComponent, TransformComponent>();
	const auto& modelComponent = object.get<ModelComponent>();


	for (auto entity : cameraView) {
		Object cameraEntity(entity);

		modelComponent.shader->setMat4("view", getViewMatrix(cameraEntity));
		modelComponent.shader->setMat4("projection", getProjectionMatrix(cameraEntity));
		modelComponent.shader->setMat4("model", TransformSystem::getModelMatrix(transformComponent));
		modelComponent.model.draw(*modelComponent.shader);
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

void glg::RendererSystem::setLightUniforms(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, int lightType, int index, int size)
{
	for (Shader* shader : shaders::getShaders()) {
		shader->setInt("lightsSize", size);

		shader->setVec4("lights[" + std::to_string(index) + "].position", glm::vec4(position, lightType));
		shader->setVec3("lights[" + std::to_string(index) + "].direction", direction);
		shader->setVec3("lights[" + std::to_string(index) + "].ambient", ambient);
		shader->setVec3("lights[" + std::to_string(index) + "].diffuse", diffuse);
		shader->setVec3("lights[" + std::to_string(index) + "].specular", specular);
		shader->setFloat("lights[" + std::to_string(index) + "].constant", constant);
		shader->setFloat("lights[" + std::to_string(index) + "].linear", linear);
		shader->setFloat("lights[" + std::to_string(index) + "].quadratic", quadratic);
		shader->setFloat("lights[" + std::to_string(index) + "].cutOff", glm::cos(glm::radians(innerCutOff)));
		shader->setFloat("lights[" + std::to_string(index) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
	}
}
