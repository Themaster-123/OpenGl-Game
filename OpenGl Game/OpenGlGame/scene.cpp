#include "scene.h"

using namespace glg;

std::vector<ComponentSystem*>& scene::getGlobalSystems()
{
	static std::vector<ComponentSystem*> vector;
	return vector;
}

void scene::callPhysicsUpdate()
{
	auto& systems = getGlobalSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->globalPhysicsUpdate();

		for (Scene* scene : Scene::SCENES) {
			systems[i]->physicsUpdate(scene);
		}
	}
}

/*std::vector<ComponentSystem*>& glg::scene::Scene::getSystems()
{
	static std::vector<ComponentSystem*> vector;
	return vector;
}*/

glg::scene::Scene::Scene() : registry()
{
	auto& systems = getGlobalSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->registerDependencies(this);
	}
}
