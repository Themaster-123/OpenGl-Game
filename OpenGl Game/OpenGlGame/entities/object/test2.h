#pragma once
#include "../visible_entity.h"

namespace glg {
	class Test2 : public VisibleEntity
	{
	public:
		Test2(glm::vec3 position, glm::quat rotation);

		Test2(glm::vec3 position, glm::vec3 rotation);

	protected:
		virtual Shader& getShader();

		virtual Model& getModel();
	};
}


