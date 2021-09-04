#include "attenuation_light_entity.h"

glg::AttenuationLightEntity::AttenuationLightEntity(glm::vec3 position, glm::quat rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float priority) :
	LightEntity(position, rotation, ambient, diffuse, specular, priority), constant(constant), linear(linear), quadratic(quadratic)
{
}

glg::AttenuationLightEntity::AttenuationLightEntity(glm::vec3 position, glm::vec3 rotation, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float priority) : 
	LightEntity(position, rotation, ambient, diffuse, specular, priority), constant(constant), linear(linear), quadratic(quadratic)
{
}
