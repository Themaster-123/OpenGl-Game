#pragma once
#include "../resources/model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "entity.h"
#include "../globals/shaders.h"
#include "../globals/models.h"

namespace glg {
	class VisibleEntity : public Entity
	{
	public:
		VisibleEntity(glm::vec3 position, glm::quat rotation);

		VisibleEntity(glm::vec3 position, glm::vec3 rotation);

		virtual glm::mat4 getModelMatrix() const;

		virtual void draw();

		virtual void update();
		
	protected:

		virtual Shader& getShader() = 0;

		virtual Model& getModel() = 0;

	};
}

