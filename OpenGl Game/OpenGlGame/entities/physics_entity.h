#pragma once
#include "visible_entity.h"
#include <reactphysics3d/reactphysics3d.h>
#include "../physics.h"

namespace glg {
	class PhysicsEntity : public VisibleEntity
	{
	public:
		PhysicsEntity(glm::vec3 position, glm::quat rotation);

		PhysicsEntity(glm::vec3 position, glm::vec3 rotation);

		virtual void setPosition(glm::vec3 position);

		virtual void setRotation(glm::quat rotation);

		virtual rp3d::Transform getTransform() const;

		virtual rp3d::Transform getInterpolatedTransform() const;

		virtual glm::mat4 getModelMatrix() const;

	protected:
		rp3d::CollisionBody* collisionBody;
		rp3d::Transform prevTransform;

		virtual rp3d::CollisionBody* getCollisionBody() = 0;

		virtual void update();

		virtual void physicsUpdate();

		virtual void updatePositionToBodyPosition(rp3d::Transform transform);

		virtual void updateRotationToBodyRotation(rp3d::Transform transform);
	};
}

