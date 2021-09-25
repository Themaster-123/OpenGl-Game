#include "world.h"
#include "../screen.h"

glm::vec3 glg::world::CHUNK_SIZE = glm::vec3(32, 32, 32);

unsigned int glg::world::CHUNK_LOAD_SIZE = 4;

glm::ivec3 glg::world::CHUNK_RESOLUTION = glm::ivec3(32, 32, 32);

glg::world::NoiseSettings glg::world::NOISE_SETTINGS;

void glg::world::setNoiseSetting()
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.05);
	noise.SetFractalOctaves(5);
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

glg::world::World::World() : chunks()
{
}

void glg::world::World::loadChunk(chunkVec chunkPos)
{
	if (!isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		chunks.insert(std::pair<chunkVec, std::shared_ptr<Chunk>>(chunkPos, std::make_shared<Chunk>(chunkPos)));
		chunksMutex.unlock();
	}
}

void glg::world::World::loadChunk(chunkVec chunkPos, std::shared_ptr<Model> model, rp3d::TriangleVertexArray* triangleArray, rp3d::TriangleMesh* triangleMesh, rp3d::ConcaveMeshShape* concaveMesh)
{
	if (!isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		chunks.insert(std::pair<chunkVec, std::shared_ptr<Chunk>>(chunkPos, std::make_shared<Chunk>(chunkPos, model, triangleArray, triangleMesh, concaveMesh)));
		chunksMutex.unlock();
	}
}

void glg::world::World::unloadChunk(const chunkVec& chunkPos)
{
	if (isChunkLoaded(chunkPos)) {
		chunksMutex.lock();
		chunks.erase(chunkPos);
		chunksMutex.unlock();
	}
}

bool glg::world::World::isChunkLoaded(const chunkVec& chunkPos) const {
	chunksMutex.lock();
	bool contains = chunks.contains(chunkPos);
	chunksMutex.unlock();
	return contains;
}

chunkVec glg::world::World::getChunkPosition(glm::vec3 position)
{
	chunkVec chunkPos;

	if (position.x < 0) {
		chunkPos.x = int(std::floor(position.x / CHUNK_SIZE.x));
	}
	else {
		chunkPos.x = int(position.x / CHUNK_SIZE.x);
	}

	if (position.z < 0) {
		chunkPos.y = int(std::floor(position.z / CHUNK_SIZE.z));
	}
	else {
		chunkPos.y = int(position.z / CHUNK_SIZE.z);
	}

	return chunkPos;
}
