#include "physics.h"
#include "screen.h"
#include <reactphysics3d/reactphysics3d.h> 
#include <iostream> 
#include "scene.h"
#include "components/components.h"

rp3d::PhysicsCommon glg::PHYSICS_COMMON;
rp3d::PhysicsWorld* glg::PHYSICS_WORLD;
const float glg::PHYSICS_TIME_STEP = 1.0f / 60.0f;
float glg::FACTOR = 0;
float glg::ACCUMULATOR = 0;
std::mutex glg::PHYSICS_MUTEX;


void glg::registerPhysics()
{
	PHYSICS_WORLD = PHYSICS_COMMON.createPhysicsWorld();
}

void glg::physicsFrame()
{
	ACCUMULATOR += DELTA_TIME;

	while (ACCUMULATOR >= PHYSICS_TIME_STEP) {
		PHYSICS_MUTEX.lock();
		PHYSICS_WORLD->update(PHYSICS_TIME_STEP);
		scene::callPhysicsUpdate();
		PHYSICS_MUTEX.unlock();

		ACCUMULATOR -= PHYSICS_TIME_STEP; 
	}

	FACTOR = ACCUMULATOR / PHYSICS_TIME_STEP;
}

rp3d::Vector3 glg::toVector3(glm::vec3 vec3)
{
	return rp3d::Vector3(vec3.x, vec3.y, vec3.z);
}

rp3d::Quaternion glg::toQuaternion(glm::quat quat)
{
	return rp3d::Quaternion(quat.x, quat.y, quat.z, quat.w);
}
