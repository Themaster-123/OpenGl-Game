#pragma once
#include "../../scene.h"

namespace glg {
	struct SceneComponent {
		Scene* scene;

		SceneComponent(Scene* scene);

		SceneComponent& operator=(const SceneComponent& other);
	};
}