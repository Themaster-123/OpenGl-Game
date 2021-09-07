#pragma once
#include <entt/entt.hpp>

namespace glg {
	class Object
	{
	public:
		Object();

		~Object();

		entt::entity getEntityId();

		template<typename Component, typename... Args>
		decltype(auto) addComponent(Component component, Args**... args);

	private:
		entt::entity entityId;

	};

	template<typename Component, typename... Args>
	decltype(auto) Object::addComponent(Component component, Args**... args)
	{
		return scene::REGISTRY.emplace<component>(entityId, args);
	}
}
