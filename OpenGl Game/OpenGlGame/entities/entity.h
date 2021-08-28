#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
//#include "../../screen.h"

namespace glg {
	class Entity
	{
	public:
		Entity(glm::vec3 position, glm::quat rotation);

		Entity(glm::vec3 position, glm::vec3 rotation);

		Entity(const Entity& other);

		~Entity();

		virtual glm::vec3 getPosition();

		virtual void setPosition(glm::vec3 position);

		virtual glm::quat getRotation();

		virtual void setRotation(glm::quat rotation);

		void setRotation(glm::vec3 rotation);

		virtual glm::vec3 getFront();

		virtual glm::vec3 getUp();

		virtual glm::vec3 getRight();

		virtual void move(const glm::vec3& direction);

		virtual bool operator==(const Entity& other);

		virtual glm::vec2 getLookRotation();

		virtual void setLookRotation(glm::vec2 rotation);


	protected:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec2 lookRotation = glm::vec2(0.0f, 0.0f);

		virtual void updateVectors();

		virtual void update();

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

		virtual void addEntityToUpdateCycle();

		friend void loopThroughEntitys();

		friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	};
}

