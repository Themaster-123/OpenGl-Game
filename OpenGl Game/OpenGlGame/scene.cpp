#include "scene.h"

using namespace glg;

entt::dispatcher Scene::DISPATCHER;

std::vector<Scene*> Scene::SCENES;

std::vector<ComponentSystem*>& Scene::getGlobalSystems()
{
	static std::vector<ComponentSystem*> vector;
	return vector;
}

void Scene::callPhysicsUpdate()
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

glg::Scene::Scene() : registry()
{
	SCENES.push_back(this);
	auto& systems = getGlobalSystems();

	for (int i = 0; i < systems.size(); i++) {
		systems[i]->registerDependencies(this);
	}

	registry.set<Scene*>(this);
}

glg::Scene::~Scene()
{
	std::remove(SCENES.begin(), SCENES.end(), this);
	registry.clear();
}

void glg::Scene::clear()
{
	registry.clear();
}
