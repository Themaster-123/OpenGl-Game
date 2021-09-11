#pragma once
#include "../components.h"
#include <reactphysics3d/reactphysics3d.h>

namespace glg {
	struct PhysicsComponent {
		rp3d::CollisionBody* collisionBody;
		TransformComponent prevTransform;

		PhysicsComponent() = default;

		PhysicsComponent(rp3d::CollisionBody* collisionBody);;

	};
}