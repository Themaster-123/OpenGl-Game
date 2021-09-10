#pragma once
#include "../../essential/object.h"
#include "../../scene.h"
#include "../transform_component/transform_component.h"
#include "../components.h"
#include <entt/entt.hpp>
#include <iostream>

namespace glg {
	class ModelComponent {
	public:
		ModelComponent() = default;

		ModelComponent(Model& model, Shader& shader);

		virtual void draw(const glm::mat4& modelMatrix) const;

		virtual ModelComponent& operator=(const ModelComponent& other);

	protected:
		Model& model;
		Shader& shader;

	};
}
