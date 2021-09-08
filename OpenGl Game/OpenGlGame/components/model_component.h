#pragma once
#include "components.h"
#include "../essential/object.h"
#include <entt/entt.hpp>
#include <iostream>
#include "../scene.h"
#include "testComponent.h"

namespace glg {
	class ModelComponent : testComponent {
	public:
		ModelComponent() = default;

		ModelComponent(Object& object, Model& model, Shader& shader);

		virtual glm::mat4 getModelMatrix() const;

		virtual void draw() const;

		virtual ModelComponent& operator=(const ModelComponent& other);

	protected:
		Object* object;
		Model& model;
		Shader& shader;

	};
}
