#include "world.h"
#include "../screen.h"

glm::vec3 glg::world::CHUNK_SIZE = glm::vec3(32);

unsigned int glg::world::CHUNK_LOAD_SIZE = 4;

glm::ivec3 glg::world::CHUNK_RESOLUTION = glm::ivec3(32);

glg::world::NoiseSettings glg::world::NOISE_SETTINGS;

void glg::world::setNoiseSetting()
{
	/*FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.02);
	noise.SetFractalOctaves(5);
	noise.SetFractalGain(0.5);
	noise.SetFractalLacunarity(2.00);
	noise.SetFractalType(noise.FractalType_FBm);*/
	FastNoise::SmartNode<> noise = FastNoise::NewFromEncodedNodeTree("EABcjyJAEQACAAAAAAAgQBAAAAAAQBkAEwDD9Sg/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPwEEAAAAAAAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM3MTD4AMzMzPwAAAAA/AAAAgD8=");
	//FastNoise::SmartNode<> noise = FastNoise::New<FastNoise::Simplex>();
	NOISE_SETTINGS = NoiseSettings(noise, 78);
}

glg::world::NoiseSettings::NoiseSettings(FastNoise::SmartNode<>& noise, float displacementHeight)
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

	if (position.y < 0) {
		chunkPos.y = int(std::floor(position.y / CHUNK_SIZE.y));
	}
	else {
		chunkPos.y = int(position.y / CHUNK_SIZE.y);
	}

	if (position.z < 0) {
		chunkPos.z = int(std::floor(position.z / CHUNK_SIZE.z));
	}
	else {
		chunkPos.z = int(position.z / CHUNK_SIZE.z);
	}

	return chunkPos;
}

int glg::world::World::getChunkDistance(const chunkVec& chunkPos1, const chunkVec& chunkPos2)
{
	chunkVec pos = chunkPos1 - chunkPos2;
	return std::max(std::max(abs(pos.x), abs(pos.y)), abs(pos.z));
}

bool glg::world::DistanceCompare(const chunkVec& vec1, const chunkVec& vec2) {
	return glg::world::World::getChunkDistance(chunkVec(0), vec1) < glg::world::World::getChunkDistance(chunkVec(0), vec2);
}

float glg::world::NoiseSettings::getNoise(float x, float y, float z, float frequency, int seed)
{
	return noise->GenSingle3D(x * frequency, y * frequency, z * frequency, seed);
}

std::vector<float> glg::world::NoiseSettings::GenUniformNoise3D(glm::ivec3 start, glm::ivec3 size, float frequency, int seed)
{
	std::vector<float> tempOutput(size.x * size.y * size.z);
	std::vector<float> output;
	output.reserve(size.x * size.y * size.z);

	noise->GenUniformGrid3D(tempOutput.data(), start.x, start.y, start.z, size.x, size.y, size.z, frequency, seed);

	for (int i = 0, z = 0; z < size.x; z++) {
		for (int y = 0; y < size.y; y++) {
			for (int x = 0; x < size.z; x++, i++) {
				output.push_back(tempOutput[i]);
			}
		}
	}

	return output;
}
