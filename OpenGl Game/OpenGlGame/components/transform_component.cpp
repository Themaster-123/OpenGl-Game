#include "transform_component.h"
#include <iostream>

using namespace glg;

glg::TransformComponent::TransformComponent()
{
	setPosition(glm::vec3());
	setRotation(glm::identity<glm::quat>());
}

glg::TransformComponent::TransformComponent(glm::vec3 position, glm::quat rotation)
{
	setPosition(position);
	setRotation(rotation);
}

glg::TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 rotation)
{
	setPosition(position);
	setRotation(rotation);
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

bool TransformComponent::operator==(const TransformComponent& other) const
{
	if (this->position == other.position && this->rotation == other.rotation) {
		return true;
	}
}

void TransformComponent::updateVectors()
{
	front = glm::normalize(getRotation() * glm::vec3(0, 0, -1));
	right = glm::normalize(getRotation() * glm::vec3(1, 0, 0));
	up = glm::normalize(getRotation() * glm::vec3(0, 1, 0));
}
