#include "transform_system.h"
#include "../../scene.h"

glg::TransformSystem::TransformSystem() : ComponentSystem()
{
}

void glg::TransformSystem::setPosition(Object& object, glm::vec3 position)
{
	auto& transformComponent = object.get<TransformComponent>();
	transformComponent.position = position;
	scene::DISPATCHER.trigger<TransformSystem::onTransformUpdate>(object, transformComponent);
}

void glg::TransformSystem::setRotation(Object& object, glm::quat rotation)
{
	auto& transformComponent = object.get<TransformComponent>();
	transformComponent.rotation = rotation;
	updateVectors(object);
	scene::DISPATCHER.trigger<TransformSystem::onTransformUpdate>(object, transformComponent);
}

void glg::TransformSystem::setRotation(Object& object, glm::vec3 rotation)
{
	setRotation(object, glm::quat(glm::radians(rotation)));

}

void glg::TransformSystem::setTransform(Object& object, TransformComponent& transform)
{
	auto& transformComponent = object.get<TransformComponent>();
	transformComponent.position = transform.position;
	transformComponent.rotation = transform.rotation;
	updateVectors(object);
	
	scene::DISPATCHER.trigger<TransformSystem::onTransformUpdate>(object, transformComponent);
}

void glg::TransformSystem::updateVectors(Object& object)
{
	auto& transform = object.get<TransformComponent>();
	transform.front = glm::normalize(transform.rotation * glm::vec3(0, 0, -1));
	transform.right = glm::normalize(transform.rotation * glm::vec3(1, 0, 0));
	transform.up = glm::normalize(transform.rotation * glm::vec3(0, 1, 0));
}

glm::mat4 glg::TransformSystem::getModelMatrix(const TransformComponent& transform)
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, transform.position);
	modelMatrix = modelMatrix * glm::toMat4(transform.rotation);
	return modelMatrix;
}

glg::TransformComponent glg::TransformSystem::interpolateTransforms(const TransformComponent& prevTransform, const TransformComponent& currentTransform, float factor)
{
	glm::vec3 position = prevTransform.position * (1.0f - factor) + currentTransform.position * factor;
	glm::quat rotation = glm::slerp(prevTransform.rotation, currentTransform.rotation, factor);
	return TransformComponent(position, rotation);
}
