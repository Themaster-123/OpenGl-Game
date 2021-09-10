#include "transform_system.h"

glg::TransformSystem::TransformSystem() : ComponentSystem()
{
}

void glg::TransformSystem::setPosition(Object& object, glm::vec3 position)
{
	object.get<TransformComponent>().position = position;
}

void glg::TransformSystem::setRotation(Object& object, glm::quat rotation)
{
	object.get<TransformComponent>().rotation = rotation;
	updateVectors(object);
}

void glg::TransformSystem::setRotation(Object& object, glm::vec3 rotation)
{
	setRotation(object, glm::quat(glm::radians(rotation)));
}

void glg::TransformSystem::setTransform(Object& object, TransformComponent& transform)
{
	setPosition(object, transform.position);
	setRotation(object, transform.rotation);
}

void glg::TransformSystem::updateVectors(Object& object)
{
	auto& transform = object.get<TransformComponent>();
	transform.front = glm::normalize(transform.rotation * glm::vec3(0, 0, -1));
	transform.right = glm::normalize(transform.rotation * glm::vec3(1, 0, 0));
	transform.up = glm::normalize(transform.rotation * glm::vec3(0, 1, 0));
}
