#include "object.h"
#include "../scene.h"

using namespace glg;

Object::Object() : scene(&scene::Scene::SCENES[0])
{
	entityId = scene->registry.create();
}

glg::Object::Object(scene::Scene* scene) : scene(scene)
{
	entityId = this->scene->registry.create();
}

glg::Object::Object(entt::entity entity, scene::Scene* scene) : scene(scene)
{
	entityId = entity;
}

glg::Object::Object(const Object& obj)
{
	this->scene = obj.scene;
	this->entityId = obj.entityId;
}

Object::~Object()
{
}

entt::entity Object::getEntityId()
{
	return entityId;
}

void glg::Object::destory()
{
	scene->registry.destroy(entityId);
}

bool glg::Object::isValid()
{
	return scene->registry.valid(entityId);
}
