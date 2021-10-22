#include "scene_component.h"

glg::SceneComponent::SceneComponent(Scene* scene)
{
	this->scene = scene;
}

glg::SceneComponent& glg::SceneComponent::operator=(const SceneComponent& other)
{
	this->scene = other.scene;
}

