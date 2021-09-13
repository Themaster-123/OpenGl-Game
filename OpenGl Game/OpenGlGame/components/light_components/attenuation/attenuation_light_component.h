#pragma once

namespace glg {
	struct AttenuationLightComponent
	{
		float constant;
		float linear;
		float quadratic;

		AttenuationLightComponent(float constant = 1.0f, float linear = .07f, float quadratic = .017f);

		virtual AttenuationLightComponent& operator=(const AttenuationLightComponent& other);
	};
}

