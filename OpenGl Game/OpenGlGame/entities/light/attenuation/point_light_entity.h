#pragma once
#include "attenuation_light_entity.h".h"

namespace glg {
	class PointLightEntity : public AttenuationLightEntity
	{
	public:
		PointLightEntity(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float priority = 0);

		void setShaderLightUniforms(Shader* shader, int index) const;
	};
}

