#include "components.h"

using namespace glg;

glg::ModelComponent::ModelComponent(Object& object, Model& model, Shader& shader) : model(model), shader(shader)
{
	this->object = &object;
	object.getOrAddComponent<TransformComponent>();
}

glm::mat4 glg::ModelComponent::getModelMatrix() const
{
	TransformComponent& transform = object->get<TransformComponent>();
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, transform.getPosition());
	modelMatrix = modelMatrix * glm::toMat4(transform.getRotation());
	return modelMatrix;
}


void glg::ModelComponent::draw() const
{
	shader.setMat4("model", getModelMatrix());
	model.draw(shader);
}

ModelComponent& glg::ModelComponent::operator=(const ModelComponent& other) {
	object = other.object;
	model = other.model;
	shader = other.shader;
	return *this;
};
