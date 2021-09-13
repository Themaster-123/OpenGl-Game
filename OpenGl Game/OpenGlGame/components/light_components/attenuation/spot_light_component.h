#pragma once

namespace glg {
	struct SpotLightComponent
	{
		float innerCutOff;
		float outerCutOff;

		SpotLightComponent(float innerCutOff = 12.5f, float outerCutOff = 22.5f);

		virtual SpotLightComponent& operator=(const SpotLightComponent& other);
	};
}

