#include "scene.h"

using namespace glg;

entt::registry scene::REGISTRY;
entt::dispatcher scene::DISPATCHER;
FastNoiseLite scene::NOISE;

std::vector<ComponentSystem*>& scene::getSystems()
{
	static std::vector<ComponentSystem*> vector;
	return vector;
}

void scene::callPhysicsUpdate()
{
	auto systems = scene::getSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->physicsUpdate();
	}
}

void glg::scene::setNoiseSetting()
{
	NOISE.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	NOISE.SetFrequency(0.05);
	NOISE.SetFractalOctaves(9);
	NOISE.SetFractalGain(0.3);
	NOISE.SetFractalLacunarity(2.00);
	NOISE.SetFractalType(NOISE.FractalType_FBm);
}
