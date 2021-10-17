#include "world_component.h"

glg::WorldComponent::WorldComponent() : chunks(), chunkModels(), chunksMutex()
{
	FastNoise::SmartNode<> noise = FastNoise::NewFromEncodedNodeTree("EABcjyJAEQACAAAAAAAgQBAAAAAAQBkAEwDD9Sg/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPwEEAAAAAAAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM3MTD4AMzMzPwAAAAA/AAAAgD8=");
	this->noiseSettings = NoiseSettings(noise);
}

glg::WorldComponent::WorldComponent(NoiseSettings noiseSettings) : chunks(), chunkModels(), chunksMutex(), noiseSettings(noiseSettings)
{
}

glg::WorldComponent& glg::WorldComponent::operator=(const WorldComponent& other)
{
	this->chunkModels = other.chunkModels;
	this->chunks = other.chunks;
	this->noiseSettings = other.noiseSettings;
	return *this;
}

glg::Chunk::Chunk(chunkVec position, int modelExtent) : position(position), modelExtent(modelExtent)
{
}

glg::NoiseSettings::NoiseSettings(FastNoise::SmartNode<>& noise)
{
	this->noise = noise;
}

/*void glg::world::setNoiseSetting()
{
	FastNoiseLite noise;
	//noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	//noise.SetFrequency(0.02);
	//noise.SetFractalOctaves(5);
	//noise.SetFractalGain(0.5);
	//noise.SetFractalLacunarity(2.00);
	//noise.SetFractalType(noise.FractalType_FBm);
	FastNoise::SmartNode<> noise = FastNoise::NewFromEncodedNodeTree("EABcjyJAEQACAAAAAAAgQBAAAAAAQBkAEwDD9Sg/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPwEEAAAAAAAAAEBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAM3MTD4AMzMzPwAAAAA/AAAAgD8=");
	//FastNoise::SmartNode<> noise = FastNoise::New<FastNoise::Simplex>();
	NOISE_SETTINGS = NoiseSettings(noise, 78);
}*/

float glg::NoiseSettings::getNoise(float x, float y, float z, float frequency, int seed)
{
	return noise->GenSingle3D(x * frequency, y * frequency, z * frequency, seed);
}

std::vector<float> glg::NoiseSettings::GenUniformNoise3D(glm::ivec3 start, glm::ivec3 size, float frequency, int seed)
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

