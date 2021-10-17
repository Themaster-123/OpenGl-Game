#include "world_component.h"

glg::WorldComponent::WorldComponent() : chunks(), chunkModels()
{
}

glg::WorldComponent& glg::WorldComponent::operator=(const WorldComponent& other)
{
	this->chunkModels = other.chunkModels;
	this->chunks = other.chunks;
	return *this;
}
