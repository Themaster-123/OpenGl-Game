#include "sphere_entity.h"

using namespace glg;

glg::SphereEntity::SphereEntity(glm::vec3 position, glm::quat rotation) : PhysicsEntity(position, rotation)
{
	this->collisionBody = getCollisionBody();
	setPosition(position);
	setRotation(rotation);
}

glg::SphereEntity::SphereEntity(glm::vec3 position, glm::vec3 rotation) : PhysicsEntity(position, rotation)
{
	this->collisionBody = getCollisionBody();
	setPosition(position);
	setRotation(rotation);
}

Shader& glg::SphereEntity::getShader()
{
	return shaders::defaultShader;
}

Model& glg::SphereEntity::getModel()
{
	return models::sphereModel;
}

rp3d::CollisionBody* glg::SphereEntity::getCollisionBody()
{
	rp3d::Transform transform = getTransform();
	rp3d::RigidBody* body = physicsWorld->createRigidBody(transform);
	body->addCollider(physicsCommon.createSphereShape(1.0f), rp3d::Transform::identity());
	return body;
}
