#pragma once
#include "../../components/components.h"

namespace glg {
	class PlayerSystem : public ComponentSystem {
	public:
		PlayerSystem();

		virtual void update(Scene* scene);

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos, Scene* scene);

		void registerDependencies(Scene* scene);

		static void onConstruct(entt::registry& registry, entt::entity entity);
	};
}
