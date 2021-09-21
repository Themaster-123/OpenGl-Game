#include "world.h"
#include "../screen.h"

float glg::world::CHUNK_SIZE = 32;

unsigned int glg::world::CHUNK_LOAD_SIZE = 4;

size_t glg::world::CHUNK_RESOLUTION = 32;

glg::world::NoiseSettings glg::world::NOISE_SETTINGS;

void glg::world::setNoiseSetting()
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.05);
	noise.SetFractalOctaves(9);
	noise.SetFractalGain(0.3);
	noise.SetFractalLacunarity(2.00);
	noise.SetFractalType(noise.FractalType_FBm);
	NOISE_SETTINGS = NoiseSettings(noise, 8);
}

glg::world::NoiseSettings::NoiseSettings(FastNoiseLite& noise, float displacementHeight)
{
	this->noise = noise;
	this->displacementHeight = displacementHeight;
}

glg::world::World::World()
{
}

void glg::world::World::loadChunk(glm::ivec2 chunkPos)
{
	if (!isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		chunks.insert(std::pair<glm::ivec2, std::shared_ptr<Chunk>>(chunkPos, std::make_shared<Chunk>(chunkPos)));
		chunksMutex.unlock();
	}
}

void glg::world::World::loadChunk(glm::ivec2 chunkPos, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh)
{
	if (!isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		chunks.insert(std::pair<glm::ivec2, std::shared_ptr<Chunk>>(chunkPos, std::make_shared<Chunk>(chunkPos, model, triangleArray, triangleMesh, concaveMesh)));
		chunksMutex.unlock();
	}
}

void glg::world::World::unloadChunk(const glm::ivec2& chunkPos)
{
	if (isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		std::cout << chunkPos.x << " " << chunkPos.y << chunks.contains(chunkPos) << std::endl;
		chunks.erase(chunkPos);
		chunksMutex.unlock();
	}
}

bool glg::world::World::isChunkLoaded(const glm::ivec2& chunkPos) const {
	chunksMutex.lock();
	bool contains = chunks.contains(chunkPos);
	chunksMutex.unlock();
	return contains;
}

glm::ivec2 glg::world::World::getChunkPosition(glm::vec3 position)
{
	glm::ivec2 chunkPos;

	if (position.x < 0) {
		chunkPos.x = int(std::floor(position.x / CHUNK_SIZE));
	}
	else {
		chunkPos.x = int(position.x / CHUNK_SIZE);
	}

	if (position.z < 0) {
		chunkPos.y = int(std::floor(position.z / CHUNK_SIZE));
	}
	else {
		chunkPos.y = int(position.z / CHUNK_SIZE);
	}

	return chunkPos;
}
