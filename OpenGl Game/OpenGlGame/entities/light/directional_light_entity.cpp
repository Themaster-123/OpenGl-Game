#include "directional_light_entity.h"
#include <string>

glg::DirectionalLightEntity::DirectionalLightEntity(glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority) :
	LightEntity(glm::vec3(0), rotation, ambient, diffuse, specular, priority)
{
}

glg::DirectionalLightEntity::DirectionalLightEntity(glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float priority) :
	LightEntity(glm::vec3(0), rotation, ambient, diffuse, specular, priority)
{
}

void glg::DirectionalLightEntity::setShaderLightUniforms(Shader* shader, int index) const
{
	shader->setVec4("lights[" + std::to_string(index) + "].position", glm::vec4(getPosition(), 0));
	shader->setVec3("lights[" + std::to_string(index) + "].direction", getFront());
	shader->setVec3("lights[" + std::to_string(index) + "].ambient", ambient);
	shader->setVec3("lights[" + std::to_string(index) + "].diffuse", diffuse);
	shader->setVec3("lights[" + std::to_string(index) + "].specular", specular);
}
