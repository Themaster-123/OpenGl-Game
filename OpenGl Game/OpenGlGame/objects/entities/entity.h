#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
//#include "../../screen.h"

namespace glg {
	class Entity
	{
	public:
		Entity(glm::vec3 position, glm::quat rotation);

		Entity(glm::vec3 position, glm::vec3 rotation);

		~Entity();

		glm::vec3 getPosition();

		void setPosition(glm::vec3 position);

		glm::quat getRotation();

		void setRotation(glm::quat rotation);

		void setRotation(glm::vec3 rotation);

		glm::vec3 getFront();

		glm::vec3 getUp();

		glm::vec3 getRight();

		void move(glm::vec3 direction);

		bool operator==(const Entity& other);

	protected:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;

		void updateVectors();

		void update();

		void addEntityToUpdateCycle();

		friend void loopThroughEntitys();
	};
}

