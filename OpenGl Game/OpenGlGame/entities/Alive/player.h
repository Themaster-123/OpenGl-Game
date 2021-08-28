#pragma once
#include "../camera.h"
#include "../entity.h"

namespace glg {
	class Player : public Entity
	{
	public:
		Camera camera;
		float sensitivity;
		float speed = 3;

		Player(glm::vec3 position, glm::quat rotation, const Camera& camera, float sensitivity = 0.2f);

		Player(glm::vec3 position, glm::vec3 rotation, const Camera& camera, float sensitivity = 0.2f);

		virtual void setPosition(glm::vec3 position);

		virtual void setRotation(glm::quat rotation);

		virtual void setRotation(glm::vec3 rotation);

		virtual void setLookRotation(glm::vec2 rotation);

	protected:
		virtual void update();

		virtual void processInput();

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

	};
}


