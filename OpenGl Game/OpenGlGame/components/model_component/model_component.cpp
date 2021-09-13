#include "../components.h"

using namespace glg;

glg::ModelComponent::ModelComponent(Model& model, Shader* shader) : model(model), shader(shader)
{
}

ModelComponent& glg::ModelComponent::operator=(const ModelComponent& other) {
	model = other.model;
	shader = other.shader;
	return *this;
};
