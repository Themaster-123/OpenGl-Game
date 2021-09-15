#include "scene.h"

using namespace glg;

entt::registry scene::REGISTRY;
entt::dispatcher scene::DISPATCHER;

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
