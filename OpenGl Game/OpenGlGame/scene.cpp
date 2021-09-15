#include "scene.h"
#include "world/world.h"

using namespace glg;

entt::registry scene::REGISTRY;

entt::dispatcher scene::DISPATCHER;

world::World scene::WORLD;

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
