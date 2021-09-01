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
rp3d::Transform groundTransform(rp3d::Vector3(0, -20, 0), rp3d::Quaternion::identity());
rp3d::RigidBody* ground;
rp3d::TriangleVertexArray* triangleArray;
glg::Test test(glm::vec3(pos.x, pos.y, pos.z), glm::quat(rot.w, rot.x, rot.y, rot.z));
const float glg::physicsTimeStep = 1.0f / 60.0f;
float accumulator = 0;


void glg::registerPhysics()
{
	world = physicsCommon.createPhysicsWorld();
	body = world->createRigidBody(transform);
	ground = world->createRigidBody(groundTransform);
	ground->setType(rp3d::BodyType::STATIC);
	Mesh mesh = models::terrainModel.getMeshes()[0];
	//triangleArray = new rp3d::TriangleVertexArray()

}

void glg::physicsFrame()
{
	accumulator += DELTA_TIME;

	while (accumulator >= physicsTimeStep) {
		world->update(physicsTimeStep);
		body->applyTorque(rp3d::Vector3(.1f, 0, 0) / physicsTimeStep);
		const rp3d::Transform transform = body->getTransform();
		const rp3d::Vector3& position = transform.getPosition();
		//std::cout << position.y << std::endl;
		const rp3d::Quaternion q = transform.getOrientation();
		test.setPosition(glm::vec3(position.x, position.y, position.z));
		test.setRotation(glm::quat(q.w, q.x, q.y, q.z));

		accumulator -= physicsTimeStep;
	}
}
