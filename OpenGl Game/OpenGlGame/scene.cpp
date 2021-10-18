#include "scene.h"

using namespace glg;

std::mutex scene::PLAYER_MUTEX;

std::vector<entt::entity> scene::PLAYERS;

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

glg::scene::Scene::Scene() : registry()
{
}
