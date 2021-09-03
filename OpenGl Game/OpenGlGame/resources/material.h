#pragma once
#include <glm/glm.hpp>

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Material() : ambient(1), diffuse(1), specular(1), shininess(32) {

	}

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
	{
	}
};
