#pragma once

namespace glg {
	struct PointLightComponent
	{
		PointLightComponent() = default;

		virtual PointLightComponent& operator=(const PointLightComponent& other);
	};
}

