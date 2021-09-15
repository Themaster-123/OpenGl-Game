#include "world.h"

float glg::world::World::CHUNK_SIZE = 32.0f;

glg::world::World::World()
{
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-1, -1)))); 
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-1, -2))));
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-2, -1))));
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-2, -2))));
}
