#include "../components.h"
#include "physics_component.h"
#include "../../component_systems/transform_system/transform_system.h"

glg::PhysicsComponent::PhysicsComponent(rp3d::CollisionBody* collisionBody)
{
	this->collisionBody = collisionBody;
}
