#pragma once
#include "../light_entity.h"

namespace glg {
	class AttenuationLightEntity : public LightEntity 
	{
	public:
		float constant;
		float linear;
		float quadratic;

		AttenuationLightEntity(glm::vec3 position, glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float priority = 0);

		AttenuationLightEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float priority = 0);
	};
}