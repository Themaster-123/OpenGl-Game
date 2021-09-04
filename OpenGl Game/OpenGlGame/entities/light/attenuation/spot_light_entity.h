#pragma once
#include "attenuation_light_entity.h"

namespace glg {
	class SpotLightEntity : public AttenuationLightEntity
	{
	public:
		float innerCutOff;
		float outerCutOff;

		SpotLightEntity(glm::vec3 position, glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, float priority = 0);

		SpotLightEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, float priority = 0);

		void setShaderLightUniforms(Shader* shader, int index) const;
	};
}

