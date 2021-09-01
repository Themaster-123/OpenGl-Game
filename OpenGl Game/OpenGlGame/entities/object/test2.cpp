#include "test2.h"

using namespace glg;

glg::Test2::Test2(glm::vec3 position, glm::quat rotation) : VisibleEntity(position, rotation)
{
}

glg::Test2::Test2(glm::vec3 position, glm::vec3 rotation) : VisibleEntity(position, rotation)
{
}

Shader& glg::Test2::getShader()
{
	return shaders::defaultShader;
}

Model& glg::Test2::getModel()
{
	return models::terrainModel;
}
