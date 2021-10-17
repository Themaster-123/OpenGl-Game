#include "world_component.h"

glg::WorldComponent::WorldComponent() : chunks(), chunkModels(), chunksMutex()
{
}

glg::WorldComponent& glg::WorldComponent::operator=(const WorldComponent& other)
{
	this->chunkModels = other.chunkModels;
	this->chunks = other.chunks;
	this->chunksMutex;
	return *this;
}

glg::Chunk::Chunk(chunkVec position, int modelExtent) : position(position), modelExtent(modelExtent)
{
}
