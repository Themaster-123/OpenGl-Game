#pragma once
#include "../../scene.h"

namespace glg {
	struct SceneComponent {
		Scene* scene;

		SceneComponent();

		SceneComponent(Scene* scene);

		SceneComponent& operator=(const SceneComponent& other);
	};
}