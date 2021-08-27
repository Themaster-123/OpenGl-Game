#pragma once
#include "../camera.h"
#include "../entity.h"

namespace glg {
	class Player : public Entity
	{
	public:
		Camera camera;
		float sensitivity;

		Player(glm::vec3 position, glm::quat rotation, const Camera& camera, float sensitivity = 0.2f);

		Player(glm::vec3 position, glm::vec3 rotation, const Camera& camera, float sensitivity = 0.2f);

		void setPosition(glm::vec3 position);

		glm::quat getLookRotation();

		virtual void setRotation(glm::quat rotation);

		void setRotation(glm::vec3 rotation);

	protected:
		void update();

		void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);
	};
}


