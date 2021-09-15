#include "world.h"

float glg::world::CHUNK_SIZE = 32.0f;
glg::world::NoiseSettings glg::world::NOISE_SETTINGS;

glg::world::World::World()
{
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-1, -1)))); 
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-1, -2))));
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-2, -1))));
	chunks.insert(std::pair<glm::ivec2, Chunk*>(glm::ivec2(0, 0), new Chunk(glm::ivec2(-2, -2))));
}

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
