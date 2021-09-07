#include "physics.h"
#include "screen.h"
#include "entities/object/sphere_entity.h"
#include "entities/object/test2.h"
#include <reactphysics3d/reactphysics3d.h> 
#include <iostream> 
#include "scene.h"

rp3d::PhysicsCommon glg::PHYSICS_COMMON;
rp3d::PhysicsWorld* glg::PHYSICS_WORLD;
rp3d::Transform groundTransform(rp3d::Vector3(0, -10, 0), rp3d::Quaternion::identity());
rp3d::RigidBody* ground;
rp3d::TriangleVertexArray* triangleArray;
rp3d::TriangleMesh* triangleMesh;
rp3d::ConcaveMeshShape* concaveMesh;
glg::Mesh* mesh;
glg::Test2 test2(glm::vec3(0, -10, 0), glm::quat());
const float glg::PHYSICS_TIME_STEP = 1.0f / 60.0f;
float glg::FACTOR = 0;
float glg::ACCUMULATOR = 0;


void glg::registerPhysics()
{
	PHYSICS_WORLD = PHYSICS_COMMON.createPhysicsWorld();

	ground = PHYSICS_WORLD->createRigidBody(groundTransform);
	ground->setType(rp3d::BodyType::STATIC); 
	mesh = new Mesh(models::terrainModel.getMeshes()[0]);
	triangleArray = new rp3d::TriangleVertexArray(mesh->vertices.size(), &mesh->vertices[0], sizeof(Vertex), mesh->indices.size() / 3, &mesh->indices[0], 3 * sizeof(unsigned int),
		rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
		rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	triangleMesh = PHYSICS_COMMON.createTriangleMesh();
	triangleMesh->addSubpart(triangleArray);
	concaveMesh = PHYSICS_COMMON.createConcaveMeshShape(triangleMesh);
	float radius = 1.0f;
	rp3d::SphereShape* sphereShape = PHYSICS_COMMON.createSphereShape(radius);
	ground->addCollider(concaveMesh, rp3d::Transform::identity());
}

void glg::physicsFrame()
{
	ACCUMULATOR += DELTA_TIME;

	while (ACCUMULATOR >= PHYSICS_TIME_STEP) {
		PHYSICS_WORLD->update(PHYSICS_TIME_STEP);
		scene::loopThroughEntitiesPhysics();

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
