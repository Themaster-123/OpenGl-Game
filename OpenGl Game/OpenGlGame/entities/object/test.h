#pragma once
#include "../visible_entity.h"

namespace glg {
	class Test : public VisibleEntity
	{
	public:
		Test(glm::vec3 position, glm::quat rotation);

		Test(glm::vec3 position, glm::vec3 rotation);

	protected:
		virtual Shader& getShader();

		virtual Model& getModel();
	};
}


