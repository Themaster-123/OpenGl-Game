#pragma once
#include <entt/entt.hpp>
#include "../scene.h"


namespace glg {
	class Object
	{
	public:
		Scene* scene;

		//Object();

		Object(Scene* scene);

		Object(entt::entity entity, Scene* scene);

		Object(entt::entity entity, entt::registry& scene);

		Object(const Object& obj);

		~Object();

		entt::entity getEntityId();

		void destory();

		bool isValid();

		template <typename... Component>
		decltype(auto) get() const {
			return const_cast<entt::registry*>(&scene->registry)->get<Component...>(entityId);
		}

		template <typename... Component>
		decltype(auto) get() {
			return scene->registry.get<Component...>(entityId);
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
		static void addConstruct(Scene* scene);

		template<typename Component, typename ComponentSys>
		static void addDestroy(Scene* scene);

		template<typename Component, auto Function>
		static void addConstruct(Scene* scene);

		template<typename Component, auto Function>
		static void addDestroy(Scene* scene);

	private:
		entt::entity entityId;

	};

	template<typename Component, typename... Args>
	inline decltype(auto) Object::addComponent(Args&&... args)
	{
		return scene->registry.emplace<Component>(entityId, args...);
	}
	template<typename Component, typename ...Args>
	inline decltype(auto) Object::getOrAddComponent(Args && ...args)
	{
		return scene->registry.get_or_emplace<Component>(entityId, args...);
	}

	template<typename ...Component>
	inline bool Object::allOf() const
	{
		return scene->registry.all_of<Component...>(entityId);
	}

	template<typename ...Component>
	inline bool Object::anyOf() const
	{
		return scene->registry.any_of<Component...>(entityId);
	}

	template<typename Component, typename ComponentSys>
	inline void Object::addConstruct(Scene* scene)
	{
		scene->registry.on_construct<Component>().connect<&ComponentSys::onConstruct>();
	}

	template<typename Component, typename ComponentSys>
	inline void Object::addDestroy(Scene* scene)
	{
		scene->registry.on_destroy<Component>().connect<&ComponentSys::onDestroy>();
	}

	template<typename Component, auto Function>
	inline void Object::addConstruct(Scene* scene)
	{
		scene->registry.on_construct<Component>().connect<Function>();
	}

	template<typename Component, auto Function>
	inline void Object::addDestroy(Scene* scene)
	{
		scene->registry.on_destroy<Component>().connect<Function>();
	}
}
