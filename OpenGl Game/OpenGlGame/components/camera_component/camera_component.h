#pragma once

namespace glg {
	struct CameraComponent {
		float fov;
		float nearPlane;
		float farPlane;

		CameraComponent(float fov = 70.0f, float nearPlane = 0.01f, float farPlane = 3000.0f);

		virtual CameraComponent& operator=(const CameraComponent& other);

	};
}
