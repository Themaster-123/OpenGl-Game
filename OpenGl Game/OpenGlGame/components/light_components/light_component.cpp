#include "light_component.h"

glg::LightComponent::LightComponent(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority) : ambient(ambient), diffuse(diffuse), specular(specular), priority(priority)
{
}

glg::LightComponent& glg::LightComponent::operator=(const LightComponent& other)
{
	this->ambient = other.ambient;
	this->diffuse = other.diffuse;
	this->specular = other.specular;
	this->priority = other.priority;
	return *this;
}
