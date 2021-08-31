#include "physics.h"
#include "screen.h"
#include "entities/test.h"
#include <reactphysics3d/reactphysics3d.h> 
#include <iostream> 

rp3d::PhysicsCommon physicsCommon;
rp3d::PhysicsWorld* world;
rp3d::Vector3 pos(0, 20, 0);
rp3d::Quaternion rot = rp3d::Quaternion::identity();
rp3d::Transform transform(pos, rot);
rp3d::RigidBody* body;
glg::Test test(glm::vec3(pos.x, pos.y, pos.z), glm::quat(rot.x, rot.y, rot.z, rot.w));


void registerPhysics()
{
	world = physicsCommon.createPhysicsWorld();
	body = world->createRigidBody(transform);

}

void physicsFrame()
{
	world->update(glg::DELTA_TIME);
	const rp3d::Transform transform = body->getTransform();
	const rp3d::Vector3& position = transform.getPosition();
	//std::cout << position.y << std::endl;
	const rp3d::Quaternion q = transform.getOrientation();
	test.setPosition(glm::vec3(position.x, position.y, position.z));
	test.setRotation(glm::quat(q.w, q.x, q.y, q.z));
}
