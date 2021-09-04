#pragma once
#include "light_entity.h"

namespace glg {
	class DirectionalLightEntity : public LightEntity
	{
	public:
		DirectionalLightEntity(glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority = 0);

		DirectionalLightEntity(glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority = 0);

		void setShaderLightUniforms(Shader* shader, int index) const;
	};
}

