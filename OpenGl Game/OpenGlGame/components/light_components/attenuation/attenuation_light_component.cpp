#include "attenuation_light_component.h"

glg::AttenuationLightComponent::AttenuationLightComponent(float constant, float linear, float quadratic) : constant(constant), linear(linear), quadratic(quadratic)
{
}

glg::AttenuationLightComponent& glg::AttenuationLightComponent::operator=(const AttenuationLightComponent& other)
{
	this->constant = other.constant;
	this->linear = other.linear;
	this->quadratic = other.quadratic;
	return *this;
}
