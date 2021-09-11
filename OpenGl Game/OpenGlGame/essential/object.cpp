#include "object.h"
#include "../scene.h"

using namespace glg;

Object::Object()
{
	entityId = scene::REGISTRY.create();
}

glg::Object::Object(entt::entity entity)
{
	entityId = entity;
}

glg::Object::Object(Object& obj)
{
	entityId = obj.entityId;
}

Object::~Object()
{
}

entt::entity Object::getEntityId()
{
	return entityId;
}
