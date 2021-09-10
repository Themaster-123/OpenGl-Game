#include "../components.h"

using namespace glg;

glg::ModelComponent::ModelComponent(Model& model, Shader& shader) : model(model), shader(shader)
{
}

void glg::ModelComponent::draw(const glm::mat4& modelMatrix) const
{
	shader.setMat4("model", modelMatrix);
	model.draw(shader);
}

ModelComponent& glg::ModelComponent::operator=(const ModelComponent& other) {
	model = other.model;
	shader = other.shader;
	return *this;
};
