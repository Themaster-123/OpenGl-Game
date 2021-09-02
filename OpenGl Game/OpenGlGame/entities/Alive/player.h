#pragma once
#include "../physics_entity.h"
#include "../camera.h"


namespace glg {
	class Player : public PhysicsEntity
	{
	public:
		Camera camera;
		float sensitivity;
		float speed = 10;

		Player(glm::vec3 position, glm::quat rotation, const Camera& camera, float sensitivity = 0.2f);

		Player(glm::vec3 position, glm::vec3 rotation, const Camera& camera, float sensitivity = 0.2f);

		virtual void setPosition(glm::vec3 position);

		virtual void setRotation(glm::quat rotation);

		virtual void setRotation(glm::vec3 rotation);

		virtual void setLookRotation(glm::vec2 rotation);

		virtual glm::mat4 getModelMatrix() const;

	protected:
		virtual void update();

		virtual void physicsUpdate();

		virtual void processInput();

		virtual void onMouseMovement(float xOffset, float yOffset, float xPos, float yPos);

		virtual void draw();

		virtual Shader& getShader();

		virtual Model& getModel();

		virtual rp3d::CollisionBody* getCollisionBody();

		virtual void updateRotationToBodyRotation(rp3d::Transform transform);

		virtual void setAspectRatio();

		virtual void setShaderProperties();
	};
}


