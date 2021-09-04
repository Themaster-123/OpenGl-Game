#pragma once
#include "../entity.h"

namespace glg {
	class LightEntity : Entity
	{
	public:
		float priority;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		LightEntity(glm::vec3 position, glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority);

		LightEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority);

		
	};
}

