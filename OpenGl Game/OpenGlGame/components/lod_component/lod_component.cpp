#include "lod_component.h"

glg::LodModel::LodModel(Model* model, float distance) : model(model), distance(distance)
{
}

glg::LodComponent::LodComponent(std::vector<LodModel> lodModels) : lodModels(lodModels)
{
}

glg::LodComponent& glg::LodComponent::operator=(const LodComponent& other)
{
	this->lodModels = other.lodModels;
	return *this;
}
