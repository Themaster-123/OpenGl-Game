#include "visible_entity.h"
#include <vector>

glg::VisibleEntity::VisibleEntity(glm::vec3 position, glm::quat rotation, Model model) : Entity(position, rotation), model(model)
{
}

glg::VisibleEntity::VisibleEntity(glm::vec3 position, glm::vec3 rotation, Model model) : Entity(position, rotation), model(model)
{
}

glm::mat4 glg::VisibleEntity::getModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::toMat4(rotation) * modelMatrix;
	return modelMatrix;
}

void glg::VisibleEntity::draw()
{
	getShader().setMat4("model", getModelMatrix());
	model.draw(getShader());
}
