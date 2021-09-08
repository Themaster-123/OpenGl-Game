#pragma once

namespace glg {
	class ModelComponent {
	public:
		ModelComponent(Model model) {

		}

		void draw();

		void addDependencies(entt::registry& registry, entt::entity entity) {

		}

	protected:
		TransformComponent& transformComponent;
	};
}
