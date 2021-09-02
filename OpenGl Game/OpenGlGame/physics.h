#pragma once
#include <reactphysics3d/reactphysics3d.h>

namespace glg {
	extern const float physicsTimeStep;
	extern rp3d::PhysicsCommon physicsCommon;
	extern rp3d::PhysicsWorld* physicsWorld;
	extern float accumulator;
	extern float factor;

	void registerPhysics();

	void physicsFrame();

	rp3d::Vector3 toVector3(glm::vec3 vec3);

	rp3d::Quaternion toQuaternion(glm::quat quat);
}