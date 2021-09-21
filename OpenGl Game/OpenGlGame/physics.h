#pragma once
#include <reactphysics3d/reactphysics3d.h>

namespace glg {
	extern const float PHYSICS_TIME_STEP;
	extern rp3d::PhysicsCommon PHYSICS_COMMON;
	extern rp3d::PhysicsWorld* PHYSICS_WORLD;
	extern float ACCUMULATOR;
	extern float FACTOR;

	extern std::mutex PHYSICS_MUTEX;

	void registerPhysics();

	void physicsFrame();

	rp3d::Vector3 toVector3(glm::vec3 vec3);

	rp3d::Quaternion toQuaternion(glm::quat quat);
}