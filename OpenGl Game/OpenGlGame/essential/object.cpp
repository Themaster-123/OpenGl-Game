#include "object.h"
#include "../scene.h"

using namespace glg;

Object::Object()
{
	entityId = scene::REGISTRY.create();
}

Object::~Object()
{
	scene::REGISTRY.destroy(entityId);
}

entt::entity Object::getEntityId()
{
	return entityId;
}
