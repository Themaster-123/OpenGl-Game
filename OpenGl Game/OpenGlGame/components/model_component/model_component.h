#pragma once
#include "../../essential/object.h"
#include "../../scene.h"
#include "../transform_component/transform_component.h"
#include "../components.h"
#include <entt/entt.hpp>
#include <iostream>
#include "../../resources/model.h"

namespace glg {
	struct ModelComponent {

		Model* model;
		Shader* shader;

		ModelComponent() = default;

		ModelComponent(Model* model, Shader* shader);

		virtual ModelComponent& operator=(const ModelComponent& other);
	};
}
