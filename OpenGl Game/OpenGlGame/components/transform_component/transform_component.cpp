#include "transform_component.h"
#include <iostream>
#include "../../essential/object.h"
#include "../../component_systems/transform_system/transform_system.h"

using namespace glg;

glg::TransformComponent::TransformComponent(glm::vec3 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
	this->front = glm::normalize(rotation * glm::vec3(0, 0, -1));
	this->right = glm::normalize(rotation * glm::vec3(1, 0, 0));
	this->up = glm::normalize(rotation * glm::vec3(0, 1, 0));
}

glg::TransformComponent::TransformComponent(Object world, glm::vec3 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
	this->front = glm::normalize(rotation * glm::vec3(0, 0, -1));
	this->right = glm::normalize(rotation * glm::vec3(1, 0, 0));
	this->up = glm::normalize(rotation * glm::vec3(0, 1, 0));
}

glg::TransformComponent::TransformComponent(Object world, glm::vec3 position, glm::vec3 rotation) : TransformComponent(world, position, glm::quat(glm::radians(rotation)))
{
}

/*glg::TransformComponent::TransformComponent(const rp3d::Transform& transform)
{
	position = transform.getPosition();
	rotation = transform.getOrientation();
	this->front = glm::normalize(rotation * glm::vec3(0, 0, -1));
	this->right = glm::normalize(rotation * glm::vec3(1, 0, 0));
	this->up = glm::normalize(rotation * glm::vec3(0, 1, 0));
}*/

bool TransformComponent::operator==(const TransformComponent& other) const
{
	return this->position == other.position && this->rotation == other.rotation;
}

TransformComponent& glg::TransformComponent::operator=(const TransformComponent& other)
{
	this->position = other.position;
	this->rotation = other.rotation;
	this->front = other.front;
	this->right = other.right;
	this->up = other.up;
	return *this;
}

glg::TransformComponent::operator rp3d::Transform() const
{
	return rp3d::Transform(position, rotation);
}
