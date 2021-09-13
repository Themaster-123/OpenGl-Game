#pragma once
#include <glm/glm.hpp>

namespace glg {
	struct LightComponent
	{
		float priority;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		LightComponent(glm::vec3 ambient = glm::vec3(.1f), glm::vec3 diffuse = glm::vec3(1), glm::vec3 specular = glm::vec3(1), float priority = 0);

		virtual LightComponent& operator=(const LightComponent& other);
	};
}

