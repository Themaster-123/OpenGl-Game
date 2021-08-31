#include "test.h"

using namespace glg;

glg::Test::Test(glm::vec3 position, glm::quat rotation) : VisibleEntity(position, rotation)
{
}

glg::Test::Test(glm::vec3 position, glm::vec3 rotation) : VisibleEntity(position, rotation)
{
}

Shader& glg::Test::getShader()
{
	return shaders::defaultShader;
}

Model& glg::Test::getModel()
{
	return models::defaultModel;
}
