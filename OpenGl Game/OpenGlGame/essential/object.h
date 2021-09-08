#pragma once
#include <entt/entt.hpp>
#include "../scene.h"
#include <entt/entt.hpp>

namespace glg {
	class Object
	{
	public:
		Object();

		Object(entt::entity entity);

		~Object();

		entt::entity getEntityId();

		template<typename Component, typename... Args>
		decltype(auto) addComponent(Args**... args);

		template<typename Component, typename Other, typename... Args>
		static void addDependency(Args**... args);

	private:
		entt::entity entityId;

	};

	template<typename Component, typename... Args>
	inline decltype(auto) Object::addComponent(Args**... args)
	{
		return scene::REGISTRY.emplace<Component>(entityId, args...);
	}

	template<typename Component, typename Other, typename... Args>
	inline void Object::addDependency(Args**... args)
	{
		scene::REGISTRY.on_construct<Component>().connect<&entt::invoke<&Component::addDependencies>>();
	}
}
