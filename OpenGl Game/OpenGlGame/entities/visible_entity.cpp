#include "visible_entity.h"
#include <vector>

glg::VisibleEntity::VisibleEntity(glm::vec3 position, glm::quat rotation) : Entity(position, rotation)
{
}

glg::VisibleEntity::VisibleEntity(glm::vec3 position, glm::vec3 rotation) : Entity(position, rotation)
{
}

glm::mat4 glg::VisibleEntity::getModelMatrix() const
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::toMat4(rotation) * modelMatrix;
	return modelMatrix;
}

void glg::VisibleEntity::draw()
{
	getShader().setMat4("model", getModelMatrix());
	getModel().draw(getShader());
}

void glg::VisibleEntity::update()
{
	draw();
	std::cout << "eaa";
}
