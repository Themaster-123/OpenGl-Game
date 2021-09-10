#include "object.h"
#include "../scene.h"

using namespace glg;

Object::Object()
{
	entityId = scene::REGISTRY.create();
	deleteEntity = true;
}

glg::Object::Object(entt::entity entity)
{
	entityId = entity;
}

glg::Object::Object(Object& obj)
{
	entityId = obj.entityId;
	deleteEntity = false;
}

Object::~Object()
{
	if (deleteEntity) {
		scene::REGISTRY.destroy(entityId);
	}
}

entt::entity Object::getEntityId()
{
	return entityId;
}
