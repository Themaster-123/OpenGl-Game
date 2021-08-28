#include "entity.h"
#include "../screen.h"
#include <iostream>

using namespace glg;

Entity::Entity(glm::vec3 position, glm::quat rotation)
{
	setPosition(position);
	setRotation(rotation);
	updateVectors();
	addEntityToUpdateCycle();
}

Entity::Entity(glm::vec3 position, glm::vec3 rotation) : Entity(position, glm::quat(glm::radians(rotation)))
{
}

Entity::Entity(const Entity& other)
{
	this->position = other.position;
	this->rotation = other.rotation;
	updateVectors();
	addEntityToUpdateCycle();
}

Entity::~Entity()
{
	glg::removeEntityFromUpdateCycle(*this);
}

glm::vec3 Entity::getPosition() const
{
	return position;
}

void Entity::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::quat Entity::getRotation() const
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
	setRotation(glm::quat(glm::radians(rotation)));
}

glm::vec3 Entity::getFront() const
{
	return front;
}

glm::vec3 Entity::getUp() const
{
	return up;
}

glm::vec3 Entity::getRight() const
{
	return right;
}

void Entity::move(const glm::vec3& direction) {
	setPosition(getPosition() + direction);
}

bool glg::Entity::operator==(const Entity& other) const {
	return this == &other;
}

glm::vec2 glg::Entity::getLookRotation() const {
	return lookRotation;
}

void glg::Entity::setLookRotation(glm::vec2 rotation) {
	lookRotation = rotation;
	setRotation(glm::vec3(rotation.x, rotation.y, 0));
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

void glg::Entity::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos)
{
}

void Entity::addEntityToUpdateCycle()
{
	glg::addEntityToUpdateCycle((*this));
}
