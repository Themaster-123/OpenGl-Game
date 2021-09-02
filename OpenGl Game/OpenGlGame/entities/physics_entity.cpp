#include "physics_entity.h"

glg::PhysicsEntity::PhysicsEntity(glm::vec3 position, glm::quat rotation) : VisibleEntity(position, rotation)
, prevTransform(getTransform())
{
}

glg::PhysicsEntity::PhysicsEntity(glm::vec3 position, glm::vec3 rotation) : VisibleEntity(position, rotation)
, prevTransform(getTransform())
{
}

void glg::PhysicsEntity::setPosition(glm::vec3 position)
{
    VisibleEntity::setPosition(position);
    collisionBody->setTransform(getTransform());
}

void glg::PhysicsEntity::setRotation(glm::quat rotation)
{
    VisibleEntity::setRotation(rotation);
    collisionBody->setTransform(getTransform());
}

rp3d::Transform glg::PhysicsEntity::getTransform() const
{
    return rp3d::Transform(getPosition(), getRotation());
}

rp3d::Transform glg::PhysicsEntity::getInterpolatedTransform() const {
    return rp3d::Transform::interpolateTransforms(prevTransform, getTransform(), std::min(factor, 1.0f));
}

glm::mat4 glg::PhysicsEntity::getModelMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1);
    rp3d::Transform transform = getInterpolatedTransform();
    modelMatrix = glm::translate(modelMatrix, (glm::vec3) transform.getPosition());
    modelMatrix = modelMatrix * glm::toMat4((glm::quat) transform.getOrientation());
    return modelMatrix;
}

void glg::PhysicsEntity::update()
{
    VisibleEntity::update();
}

void glg::PhysicsEntity::physicsUpdate()
{
    prevTransform = getTransform();
    rp3d::Transform transform = collisionBody->getTransform();
    updatePositionToBodyPosition(transform);
    updateRotationToBodyRotation(transform);
}

void glg::PhysicsEntity::updatePositionToBodyPosition(rp3d::Transform transform)
{
    position = transform.getPosition();
}

void glg::PhysicsEntity::updateRotationToBodyRotation(rp3d::Transform transform)
{
    rotation = transform.getOrientation();
}


