#pragma once
#include "../../components/components.h"

namespace glg {
	class PlayerSystem : public ComponentSystem {
	public:
		PlayerSystem();

		virtual void update();

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

		static void onConstruct(entt::registry& registry, entt::entity entity);

		static void beforeOnContruct(entt::registry& registry, entt::entity entity);
	};
}
