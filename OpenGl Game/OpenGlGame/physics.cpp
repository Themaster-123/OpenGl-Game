#include "physics.h"
#include "screen.h"
#include "entities/object/test.h"
#include "entities/object/test2.h"
#include <reactphysics3d/reactphysics3d.h> 
#include <iostream> 

rp3d::PhysicsCommon physicsCommon;
rp3d::PhysicsWorld* world;
rp3d::Vector3 pos(0, 0, 0);
rp3d::Quaternion rot = rp3d::Quaternion::identity();
rp3d::Transform transform(pos, rot);
rp3d::RigidBody* body;
rp3d::Transform groundTransform(rp3d::Vector3(0, -10, 0), rp3d::Quaternion::identity());
rp3d::RigidBody* ground;
rp3d::TriangleVertexArray* triangleArray;
rp3d::TriangleMesh* triangleMesh;
rp3d::ConcaveMeshShape* concaveMesh;
glg::Mesh* mesh;
glg::Test test(glm::vec3(pos.x, pos.y, pos.z), glm::quat(rot.w, rot.x, rot.y, rot.z));
glg::Test2 test2(glm::vec3(0, -10, 0), glm::quat());
const float glg::physicsTimeStep = 1.0f / 60.0f;
float accumulator = 0;


void glg::registerPhysics()
{
	world = physicsCommon.createPhysicsWorld();
	body = world->createRigidBody(transform);
	ground = world->createRigidBody(groundTransform);
	ground->setType(rp3d::BodyType::STATIC);
	//Mesh m = ;
	mesh = new Mesh(models::terrainModel.getMeshes()[0]);
	triangleArray = new rp3d::TriangleVertexArray(mesh->vertices.size(), &mesh->vertices[0], sizeof(Vertex), mesh->indices.size() / 3, &mesh->indices[0], 3 * sizeof(unsigned int),
		rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
		rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	triangleMesh = physicsCommon.createTriangleMesh();
	triangleMesh->addSubpart(triangleArray);
	concaveMesh = physicsCommon.createConcaveMeshShape(triangleMesh);
	//concaveMesh->setScale(rp3d::Vector3(4, 4, 4));
	float radius = 1.0f;
	rp3d::SphereShape* sphereShape = physicsCommon.createSphereShape(radius);
	ground->addCollider(concaveMesh, rp3d::Transform::identity());
	body->addCollider(sphereShape, rp3d::Transform::identity());
}

void glg::physicsFrame()
{
	accumulator += DELTA_TIME;

	while (accumulator >= physicsTimeStep) {
		world->update(physicsTimeStep);
		loopThroughEntitysPhysics();

		//body->applyTorque(rp3d::Vector3(1, 0, 0) * physicsTimeStep);
		const rp3d::Transform transform = body->getTransform();
		const rp3d::Vector3& position = transform.getPosition();
		//std::cout << position.y << std::endl;
		const rp3d::Quaternion q = transform.getOrientation();
		test.setPosition(position);
		test.setRotation(q);
		accumulator -= physicsTimeStep;
	}
}
