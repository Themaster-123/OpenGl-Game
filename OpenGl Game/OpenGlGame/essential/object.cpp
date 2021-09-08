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

Object::~Object()
{
	scene::REGISTRY.destroy(entityId);
}

entt::entity Object::getEntityId()
{
	return entityId;
}
