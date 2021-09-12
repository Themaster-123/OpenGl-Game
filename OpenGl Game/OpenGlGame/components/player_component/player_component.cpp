#include "player_component.h"

glg::PlayerComponent::PlayerComponent(float sensitivity, float speed) : sensitivity(sensitivity), speed(speed)
{
}

glg::PlayerComponent& glg::PlayerComponent::operator=(const PlayerComponent& other)
{
	this->sensitivity = other.sensitivity;
	this->speed = other.speed;
	return *this;
}
