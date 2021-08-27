#include "entity.h"

using namespace GLG;

Entity::Entity(glm::vec3 position, glm::quat rotation)
{
	setPosition(position);
	setRotation(rotation);
	updateVectors();
}

Entity::Entity(glm::vec3 position, glm::vec3 rotation)
{
	setPosition(position);
	setRotation(rotation);
	updateVectors();
}

glm::vec3 Entity::getPosition()
{
	return position;
}

void Entity::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::quat Entity::getRotation()
{
	return rotation;
}

void Entity::setRotation(glm::quat rotation)
{
	this->rotation = rotation;
	updateVectors();
}

void Entity::setRotation(glm::vec3 rotation)
{
	this->rotation = glm::quat(glm::radians(rotation));
	updateVectors();
}

glm::vec3 Entity::getFront()
{
	return front;
}

glm::vec3 Entity::getUp()
{
	return up;
}

glm::vec3 Entity::getRight()
{
	return right;
}

void Entity::move(glm::vec3 direction) {
	setPosition(getPosition() + direction);
}

void Entity::updateVectors()
{
	front = glm::normalize(getRotation() * glm::vec3(0, 0, -1));
	right = glm::normalize(getRotation() * glm::vec3(1, 0, 0));
	up = glm::normalize(getRotation() * glm::vec3(0, 1, 0));
}

void Entity::update()
{
}

void Entity::addEntityToUpdateCycle()
{
}
