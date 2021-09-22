#include "box_cull_component.h"

glg::BoxCullComponent::BoxCullComponent(glm::vec3 size, glm::vec3 offset) : size(size), offset(offset)
{
}

glg::BoxCullComponent& glg::BoxCullComponent::operator=(const BoxCullComponent& other)
{
	this->size = other.size;
	this->offset = other.offset;

	return *this;
}
