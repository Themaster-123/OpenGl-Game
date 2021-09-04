#include "scene.h"


using namespace glg;

glg::Player* scene::MAIN_PLAYER;

std::vector<LightEntity*>& scene::getLights()
{
	static std::vector<LightEntity*> lights;
	return lights;
}
