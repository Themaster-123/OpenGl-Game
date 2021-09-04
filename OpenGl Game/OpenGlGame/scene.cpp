#include "scene.h"
#include "entities/light/light_entity.cpp"

using namespace glg;

glg::Player* scene::MAIN_PLAYER;

std::vector<LightEntity*> scene::getLights()
{
	static std::vector<LightEntity*> lights;
	return lights;
}
