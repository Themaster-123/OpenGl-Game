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

		Object(Object& obj);

		~Object();

		entt::entity getEntityId();

		template <typename... Component>
		decltype(auto) get() const {
			return ((const entt::registry) scene::REGISTRY).get<Component...>(entityId);
		}

		template <typename... Component>
		decltype(auto) get() {
			return scene::REGISTRY.get<Component...>(entityId);
		}

		template<typename Component, typename... Args>
		decltype(auto) addComponent(Args&&... args);

		template<typename Component, typename... Args>
		decltype(auto) getOrAddComponent(Args&&... args);

		template<typename Component, typename ComponentSystem>
		static void addConstruct();

	private:
		entt::entity entityId;
		bool deleteEntity = false;

	};

	template<typename Component, typename... Args>
	inline decltype(auto) Object::addComponent(Args&&... args)
	{
		return scene::REGISTRY.emplace<Component>(entityId, args...);
	}
	template<typename Component, typename ...Args>
	inline decltype(auto) Object::getOrAddComponent(Args && ...args)
	{
		return scene::REGISTRY.get_or_emplace<Component>(entityId, args...);
	}
	template<typename Component, typename ComponentSys>
	inline void Object::addConstruct()
	{
		scene::REGISTRY.on_construct<Component>().connect<&ComponentSys::onConstruct>();
	}
}
