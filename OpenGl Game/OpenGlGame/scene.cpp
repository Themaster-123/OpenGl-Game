#include "scene.h"

using namespace glg;

std::vector<ComponentSystem*>& scene::getGlobalSystems()
{
	static std::vector<ComponentSystem*> vector;
	return vector;
}

void scene::Scene::callPhysicsUpdate()
{
	auto systems = getSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->physicsUpdate();
	}
}

std::vector<ComponentSystem*>& glg::scene::Scene::getSystems()
{
	static std::vector<ComponentSystem*> vector;
	return vector;
}

glg::scene::Scene::Scene() : registry()
{
}
