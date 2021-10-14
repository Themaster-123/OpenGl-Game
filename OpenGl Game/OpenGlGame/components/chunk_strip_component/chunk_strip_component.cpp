#include "chunk_strip_component.h"

glg::ChunkStripComponent::ChunkStripComponent() : chunks()
{
}

glg::ChunkStripComponent& glg::ChunkStripComponent::operator=(const ChunkStripComponent& other)
{
	this->chunks = other.chunks;
	return *this;
}
