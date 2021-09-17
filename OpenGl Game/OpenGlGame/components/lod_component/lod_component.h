#pragma once
#include <vector>
#include "../../resources/model.h"

namespace glg {
	struct LodModel {
		Model* model;
		float distance;

		LodModel(Model* model, float distance);
	};

	struct LodComponent
	{
		std::vector<LodModel> lodModels;

		LodComponent() = default;

		LodComponent(std::vector<LodModel> lodModels);

		LodComponent& operator= (const LodComponent& other);
	};

}
