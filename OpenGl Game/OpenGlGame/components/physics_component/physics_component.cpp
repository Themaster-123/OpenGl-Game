#include "../components.h"
#include "physics_component.h"

glg::PhysicsComponent::PhysicsComponent(rp3d::CollisionBody* collisionBody)
{
	this->collisionBody = collisionBody;
}

glg::TransformComponent glg::PhysicsComponent::getInterpolatedTransform(const glg::TransformComponent& transform)
{
	return TransformComponent::interpolateTransforms(prevTransform, transform, std::min(FACTOR, 1.0f));
}
