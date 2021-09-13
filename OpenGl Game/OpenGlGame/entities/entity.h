#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>

namespace glg {
	class Entity
	{
	public:
		Entity(glm::vec3 position, glm::quat rotation);

		Entity(glm::vec3 position, glm::vec3 rotation);

		Entity(const Entity& other);

		~Entity();

		virtual glm::vec3 getPosition() const;

		virtual void setPosition(glm::vec3 position);

		virtual glm::quat getRotation() const;

		virtual void setRotation(glm::quat rotation);

		virtual void setRotation(glm::vec3 rotation);

		virtual glm::vec3 getFront() const;

		virtual glm::vec3 getUp() const;

		virtual glm::vec3 getRight() const;

		virtual void move(const glm::vec3& direction);

		virtual bool operator==(const Entity& other) const;

		virtual glm::vec2 getLookRotation() const;

		virtual void setLookRotation(glm::vec2 rotation);

		virtual void update();

		virtual void physicsUpdate();
		
		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

	protected:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec2 lookRotation = glm::vec2(0.0f, 0.0f);

		virtual void updateVectors();
	};
}

