#include "spot_light_entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include "attenuation_light_entity.h"
#include <string>

glg::SpotLightEntity::SpotLightEntity(glm::vec3 position, glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, float priority) :
	AttenuationLightEntity(position, rotation, ambient, diffuse, specular, constant, linear, quadratic, priority), innerCutOff(innerCutOff), outerCutOff(outerCutOff)
{
}

glg::SpotLightEntity::SpotLightEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, float priority) :
	AttenuationLightEntity(position, rotation, ambient, diffuse, specular, constant, linear, quadratic, priority), innerCutOff(innerCutOff), outerCutOff(outerCutOff)
{
}

void glg::SpotLightEntity::setShaderLightUniforms(Shader* shader, int index) const
{
	shader->setVec4("lights[" + std::to_string(index) + "].position", glm::vec4(getPosition(), 2));
	shader->setVec3("lights[" + std::to_string(index) + "].direction", getFront());
	shader->setVec3("lights[" + std::to_string(index) + "].ambient", ambient);
	shader->setVec3("lights[" + std::to_string(index) + "].diffuse", diffuse);
	shader->setVec3("lights[" + std::to_string(index) + "].specular", specular);
	shader->setFloat("lights[" + std::to_string(index) + "].constant", constant);
	shader->setFloat("lights[" + std::to_string(index) + "].linear", linear);
	shader->setFloat("lights[" + std::to_string(index) + "].quadratic", quadratic);
	shader->setFloat("lights[" + std::to_string(index) + "].cutOff", glm::cos(glm::radians(innerCutOff)));
	shader->setFloat("lights[" + std::to_string(index) + "].outerCutOff", glm::cos(glm::radians(outerCutOff)));
}