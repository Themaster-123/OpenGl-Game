#include "transform_component.h"
#include <iostream>
#include "../../essential/object.h"
#include "../../component_systems/transform_system/transform_system.h"

using namespace glg;

glg::TransformComponent::TransformComponent()
{
	setPosition(glm::vec3());
	setRotation(glm::identity<glm::quat>());
}

glg::TransformComponent::TransformComponent(glm::vec3 position, glm::quat rotation)
{
	this->position = position;
	this->rotation = rotation;
	this->front = glm::normalize(rotation * glm::vec3(0, 0, -1));
	this->right = glm::normalize(rotation * glm::vec3(1, 0, 0));
	this->up = glm::normalize(rotation * glm::vec3(0, 1, 0));
}

glg::TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 rotation) : TransformComponent(position, glm::quat(glm::radians(rotation)))
{
}

glm::vec3 TransformComponent::getPosition() const
{
	return position;
}

void TransformComponent::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::quat TransformComponent::getRotation() const
{
	return rotation;
}

void TransformComponent::setRotation(glm::quat rotation)
{
	this->rotation = rotation;
	updateVectors();
}

void TransformComponent::setRotation(glm::vec3 rotation)
{
	setRotation(glm::quat(glm::radians(rotation)));
}

glm::vec3 TransformComponent::getFront() const
{
	return front;
}

glm::vec3 TransformComponent::getUp() const
{
	return up;
}

glm::vec3 TransformComponent::getRight() const
{
	return right;
}

glm::mat4 glg::TransformComponent::getModelMatrix() const
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, getPosition());
	modelMatrix = modelMatrix * glm::toMat4(getRotation());
	return modelMatrix;
}

bool TransformComponent::operator==(const TransformComponent& other) const
{
	return this->position == other.position && this->rotation == other.rotation;
}

TransformComponent& glg::TransformComponent::operator=(const TransformComponent& other)
{
	position = other.position;
	rotation = other.rotation;
	front = other.front;
	right = other.right;
	up = other.up;
	return *this;
}

TransformComponent glg::TransformComponent::interpolateTransforms(const TransformComponent& prevTransform, const TransformComponent& currentTransform, float factor)
{
	glm::vec3 position = prevTransform.getPosition() * (1.0f - factor) + currentTransform.getPosition() * factor;
	glm::quat rotation = glm::slerp(prevTransform.getRotation(), currentTransform.getRotation(), factor);
	return TransformComponent(position, rotation);
}

void TransformComponent::updateVectors()
{
}
