#pragma once
#include "../resources/model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "entity.h"

namespace glg {
	class VisibleEntity : public Entity
	{
	public:
		VisibleEntity(glm::vec3 position, glm::quat rotation, Model model);

		VisibleEntity(glm::vec3 position, glm::vec3 rotation, Model model);

		virtual glm::mat4 getModelMatrix();

		virtual void draw();
		
	protected:
		Model model;

		virtual Shader& getShader() = 0;

	};
}

