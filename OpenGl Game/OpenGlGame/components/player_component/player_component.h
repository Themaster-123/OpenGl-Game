#pragma once

namespace glg {
	struct PlayerComponent {
		float sensitivity;
		float speed = 10;

		PlayerComponent(float sensitivity = .2f, float speed = 100);

		virtual PlayerComponent& operator=(const PlayerComponent& other);
	};
}
