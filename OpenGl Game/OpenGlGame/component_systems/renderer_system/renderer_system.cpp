#include "renderer_system.h"
#include <iostream>
#include "../../scene.h"
#include "../../components/components.h"
#include "../transform_system/transform_system.h"
#include "../physics_system/physics_system.h"
#include "../../screen.h"
#include "../../physics.h"
#include "../../globals/shaders.h"
#include "../../globals/models.h"

glg::RendererSystem::RendererSystem() : ComponentSystem()
{
	Object::addConstruct<ModelComponent, RendererSystem>();
	Object::addConstruct<CameraComponent, &RendererSystem::onCameraConstruct>();
	Object::addConstruct<LightComponent, RendererSystem>();
	Object::addConstruct<DirectionalLightComponent, &RendererSystem::onLightConstruct>();
	Object::addConstruct<AttenuationLightComponent, &RendererSystem::onLightConstruct>();
	Object::addConstruct<PointLightComponent, &RendererSystem::onAttenuationLightConstruct>();
	Object::addConstruct<SpotLightComponent, &RendererSystem::onAttenuationLightConstruct>();
	Object::addConstruct<LodComponent, &RendererSystem::onLodConstruct>();
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

	auto cameraView = scene::REGISTRY.view<CameraComponent, TransformComponent>();

	for (auto cameraEntity : cameraView) {
		Object camObj(cameraEntity);

		auto [cameraComponent, transformComponent] = camObj.get<CameraComponent, TransformComponent>(cameraView);

		ViewFrustum frustum (cameraComponent, transformComponent);

		auto modelView = scene::REGISTRY.view<ModelComponent, TransformComponent>(entt::exclude<PhysicsComponent>);

		for (auto entity : modelView) {
			Object obj(entity);
			drawModel(obj, cameraComponent, transformComponent, frustum);
		}

		auto physicsModelView = scene::REGISTRY.view<ModelComponent, PhysicsComponent, TransformComponent>();

		for (auto entity : physicsModelView) {
			Object obj(entity);
			RendererSystem::drawPhysicsModel(obj, cameraComponent, transformComponent, frustum);
		}
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

void glg::RendererSystem::onLodConstruct(entt::registry& registry, entt::entity entity)
{
	Object obj(entity);
	obj.getOrAddComponent<ModelComponent>(models::defaultModel, shaders::defaultShader);
}

void glg::RendererSystem::drawModel(const Object& object, const CameraComponent& cameraComponent, const TransformComponent& cameraTransformComponent, ViewFrustum frustum)
{
	const auto& transformComponent = object.get<TransformComponent>();

	drawModel(object, transformComponent, cameraComponent, cameraTransformComponent, frustum);
}

void glg::RendererSystem::drawPhysicsModel(const Object& object, const CameraComponent& cameraComponent, const TransformComponent& cameraTransformComponent, ViewFrustum frustum)
{
	const auto [transformComponent, physicsComponent] = object.get<TransformComponent, PhysicsComponent>();

	auto cameraView = scene::REGISTRY.view<CameraComponent, TransformComponent>();

	drawModel(object, TransformSystem::interpolateTransforms(physicsComponent.prevTransform, transformComponent, std::min(FACTOR, 1.0f)), cameraComponent, cameraTransformComponent, frustum);
}

void glg::RendererSystem::drawModel(const Object& object, const TransformComponent& transformComponent, const CameraComponent& cameraComponent, const TransformComponent& cameraTransformComponent, ViewFrustum frustum)
{
	auto& modelComponent = object.get<ModelComponent>();




	if (object.allOf<LodComponent>()) {
		float distanceSq = glm::distance2(cameraTransformComponent.position, transformComponent.position);

		const auto& lodComponent = object.get<LodComponent>();

		const LodModel* bestLodModel = nullptr;

		for (const LodModel& model : lodComponent.lodModels) {
			if (distanceSq >= (model.minDistance * model.minDistance) && (bestLodModel == nullptr || bestLodModel->minDistance < model.minDistance)) {
				bestLodModel = &model;
			}
		}

		if (bestLodModel != nullptr) {
			modelComponent.model = bestLodModel->model;
		}

	}

	if (object.allOf<BoxCullComponent>()) {
		BoxCullComponent& boxCull = object.get<BoxCullComponent>();

		if (!frustum.isInside(transformComponent, boxCull.size, boxCull.offset)) {
			return;
		}
	}

	modelComponent.shader->setMat4("view", getViewMatrix(cameraTransformComponent));
	modelComponent.shader->setMat4("projection", getProjectionMatrix(cameraComponent));
	modelComponent.shader->setMat4("model", TransformSystem::getModelMatrix(transformComponent));
	modelComponent.model->draw(*modelComponent.shader);
	
}

glm::mat4 glg::RendererSystem::getViewMatrix(const TransformComponent& transformComponent)
{

	glm::mat4 view = glm::mat4(1);
	view = glm::translate(view, -transformComponent.position);
	view = glm::toMat4(glm::inverse(transformComponent.rotation)) * view;
	return view;
}

glm::mat4 glg::RendererSystem::getProjectionMatrix(const CameraComponent& cameraComponent)
{
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

glg::ViewFrustum::ViewFrustum(const CameraComponent& camera, const TransformComponent& transform)
{
	float tanFov = std::tan(glm::radians(camera.fov) / 2.0f);
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	float hNearPlane = (tanFov * camera.nearPlane);
	float wNearPlane = (hNearPlane * aspect);

	glm::vec3 fc = transform.position + transform.front * camera.farPlane;
	glm::vec3 nc = transform.position + transform.front * camera.nearPlane;

	glm::vec3 nw = glm::vec3(nc + -transform.right * wNearPlane + transform.up * hNearPlane) - transform.position;
	glm::vec3 ne = glm::vec3(nc + transform.right * wNearPlane + transform.up * hNearPlane) - transform.position;
	glm::vec3 se = glm::vec3(nc + transform.right * wNearPlane + -transform.up * hNearPlane) - transform.position;
	glm::vec3 sw = glm::vec3(nc + -transform.right * wNearPlane + -transform.up * hNearPlane) - transform.position;

	planes[NEARP] = ViewPlane(nc, transform.front);
	planes[FARP] = ViewPlane(fc, -transform.front);

	glm::vec3 aux, normal;

	normal = glm::normalize(glm::cross(nw, ne));
	planes[TOP] = ViewPlane(nc + transform.up * hNearPlane, normal);

	normal = glm::normalize(glm::cross(se, sw));
	planes[BOTTOM] = ViewPlane(nc - transform.up * hNearPlane, normal);

	normal = glm::normalize(glm::cross(sw, nw));
	planes[LEFT] = ViewPlane(nc - transform.right * wNearPlane, normal);

	normal = glm::normalize(glm::cross(ne, se));
	planes[RIGHT] = ViewPlane(nc + transform.right * wNearPlane, normal);
}

bool glg::ViewFrustum::isInside(const glm::vec3& point, float radius) const
{
	for (const ViewPlane& plane : planes) {
		if (glm::dot(plane.normal, point - plane.position) < -radius) {
			return false;
		}
	}
	return true;
}

bool glg::ViewFrustum::isInside(const TransformComponent& transform, glm::vec3 size, glm::vec3 offset)
{
	glm::mat4 modelMat = TransformSystem::getModelMatrix(transform);
	glm::vec3 points[8] { glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1),
		glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1) };

	int in = 0;

	for (int i = 0; i < 8; i++) {
		points[i] *= size;
		points[i] += offset;
		points[i] = glm::vec3(modelMat * glm::vec4(points[i], 1));
	}

	for (const ViewPlane& plane : planes) {
		in = 0;

		for (int p = 0; p < 8 && in == 0; p++) {
			if (glm::dot(plane.normal, points[p] - plane.position) >= 0) {
				in++;
			}
		}

		if (!in) {
			return false;
		}
	}

	return true;
}

glg::ViewFrustum::ViewPlane::ViewPlane(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal)
{
}

glg::ViewFrustum::BoundingCube::BoundingCube(glm::vec3 size, glm::vec3 offset) : points {glm::vec3(-1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, -1, 1), glm::vec3(-1, -1, 1),
glm::vec3(-1, 1, -1), glm::vec3(1, 1, -1), glm::vec3(1, -1, -1), glm::vec3(-1, -1, -1) }
{
	for (glm::vec3& point : points) {
		 point *= size;
		 point += offset;
	}
}
