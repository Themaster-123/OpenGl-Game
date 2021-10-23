#include "player_system.h"
#include <GLFW/glfw3.h>
#include "../../screen.h"
#include "../transform_system/transform_system.h"
#include "../../scene.h"

glg::PlayerSystem::PlayerSystem() : ComponentSystem()
{
}

void glg::PlayerSystem::update(Scene* scene)
{
	auto playerView = scene->registry.view<PlayerComponent, TransformComponent>();

	for (auto entity : playerView) {
		Object object(entity);

		auto [playerComponent, transformComponent] = playerView.get<PlayerComponent, TransformComponent>(entity);

		float deltaSpeed = playerComponent.speed * DELTA_TIME * 10;

		if (getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(GAME_WINDOW, true);
		}
		if (getKey(GLFW_KEY_W) == GLFW_PRESS) {
			TransformSystem::setPosition(object, transformComponent.position + glm::normalize(transformComponent.front) * deltaSpeed);
		}
		if (getKey(GLFW_KEY_S) == GLFW_PRESS) {
			TransformSystem::setPosition(object, transformComponent.position + glm::normalize(-transformComponent.front) * deltaSpeed);
		}
		if (getKey(GLFW_KEY_A) == GLFW_PRESS) {
			TransformSystem::setPosition(object, transformComponent.position + glm::normalize(-transformComponent.right) * deltaSpeed);
		}
		if (getKey(GLFW_KEY_D) == GLFW_PRESS) {
			TransformSystem::setPosition(object, transformComponent.position + glm::normalize(transformComponent.right) * deltaSpeed);
		}
		if (getKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
			TransformSystem::setPosition(object, transformComponent.position + glm::normalize(transformComponent.up) * deltaSpeed);
		}
	}
}

void glg::PlayerSystem::onMouseMovement(float xOffset, float yOffset, float xPos, float yPos, Scene* scene)
{
	auto playerView = scene->registry.view<PlayerComponent, TransformComponent, LookableComponent>();

	for (auto entity : playerView) {
		Object object(entity);

		auto [playerComponent, transformComponent, lookableComponent] = playerView.get<PlayerComponent, TransformComponent, LookableComponent>(entity);
		
		xOffset *= playerComponent.sensitivity;
		yOffset *= playerComponent.sensitivity;
		lookableComponent.yaw -= xOffset;
		lookableComponent.pitch -= yOffset;
		lookableComponent.pitch = std::max(std::min(lookableComponent.pitch, 90.0f), -90.0f);
		TransformSystem::setRotation(object, glm::vec3(lookableComponent.pitch, lookableComponent.yaw, 0));
	}
}

void glg::PlayerSystem::registerDependencies(Scene* scene)
{
	Object::addConstruct<PlayerComponent, PlayerSystem>(scene);
}

void glg::PlayerSystem::onConstruct(entt::registry& registry, entt::entity entity)
{
	//scene::PLAYER_MUTEX.lock();
	Object object(entity);
	object.getOrAddComponent<CameraComponent>();
	object.getOrAddComponent<LookableComponent>();
	//scene::PLAYERS.push_back(entity);
	//scene::PLAYER_MUTEX.unlock();
}
