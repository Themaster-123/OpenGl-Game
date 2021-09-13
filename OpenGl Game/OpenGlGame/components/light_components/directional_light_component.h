#pragma once

namespace glg {
	struct DirectionalLightComponent
	{
		DirectionalLightComponent() = default;

		virtual DirectionalLightComponent& operator=(const DirectionalLightComponent& other);
	};
}

