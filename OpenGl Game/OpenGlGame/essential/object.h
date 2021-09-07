#pragma once
#include <entt/entt.hpp>
#include "../scene.h"

namespace glg {
	class Object
	{
	public:
		Object();

		~Object();

		entt::entity getEntityId();

		template<typename Component, typename... Args>
		decltype(auto) addComponent(Args**... args);

	private:
		entt::entity entityId;

	};

	template<typename Component, typename... Args>
	decltype(auto) Object::addComponent(Args**... args)
	{
		return scene::REGISTRY.emplace<Component>(entityId, args...);
	}
}
