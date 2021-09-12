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

		void destory();

		bool isValid();

		template <typename... Component>
		decltype(auto) get() const {
			return const_cast<entt::registry*>(&scene::REGISTRY)->get<Component...>(entityId);
		}

		template <typename... Component>
		decltype(auto) get() {
			return scene::REGISTRY.get<Component...>(entityId);
		}

		template <typename... Component, typename View>
		decltype(auto) get(View view) const {
			return const_cast<View*>(&view)->get<Component...>(entityId);
		}

		template <typename... Component, typename View>
		decltype(auto) get(View view) {
			return view.get<Component...>(entityId);
		}

		template<typename Component, typename... Args>
		decltype(auto) addComponent(Args&&... args);

		template<typename Component, typename... Args>
		decltype(auto) getOrAddComponent(Args&&... args);

		template <typename... Component>
		bool allOf() const;

		template <typename... Component>
		bool anyOf() const;

		template<typename Component, typename ComponentSys>
		static void addConstruct();

		template<typename Component, typename ComponentSys>
		static void addDestroy();

		template<typename Component, auto Function>
		static void addConstruct();

		template<typename Component, auto Function>
		static void addDestroy();

	private:
		entt::entity entityId;

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

	template<typename ...Component>
	inline bool Object::allOf() const
	{
		return scene::REGISTRY.all_of<Component...>(entityId);
	}

	template<typename ...Component>
	inline bool Object::anyOf() const
	{
		return scene::REGISTRY.any_of<Component...>(entityId);
	}

	template<typename Component, typename ComponentSys>
	inline void Object::addConstruct()
	{
		scene::REGISTRY.on_construct<Component>().connect<&ComponentSys::onConstruct>();
	}

	template<typename Component, typename ComponentSys>
	inline void Object::addDestroy()
	{
		scene::REGISTRY.on_destroy<Component>().connect<&ComponentSys::onDestroy>();
	}

	template<typename Component, auto Function>
	inline void Object::addConstruct()
	{
		scene::REGISTRY.on_construct<Component>().connect<Function>();
	}

	template<typename Component, auto Function>
	inline void Object::addDestroy()
	{
		scene::REGISTRY.on_destroy<Component>().connect<Function>();
	}
}
