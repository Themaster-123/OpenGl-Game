#pragma once
#include "../physics_entity.h"

namespace glg {
	class SphereEntity : public PhysicsEntity
	{
	public:
		SphereEntity(glm::vec3 position, glm::quat rotation);

		SphereEntity(glm::vec3 position, glm::vec3 rotation);

	protected:
		virtual Shader& getShader();

		virtual Model& getModel();

		virtual rp3d::CollisionBody* getCollisionBody();
	};
}


