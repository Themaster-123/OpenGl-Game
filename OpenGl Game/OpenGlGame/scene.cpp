#include "scene.h"

using namespace glg;

glg::Player* scene::MAIN_PLAYER;

std::vector<LightEntity*>& scene::getLights()
{
	static std::vector<LightEntity*> lights;
	return lights;
}

std::vector<Entity*>& scene::getEntities() {
	static std::vector<Entity*> vector;
	return vector;
}

void scene::loopThroughEntitiesPhysics()
{
	for (Entity* entity : getEntities()) {
		entity->physicsUpdate();
	}
}
