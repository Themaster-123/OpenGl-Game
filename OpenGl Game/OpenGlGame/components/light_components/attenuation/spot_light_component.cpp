#include "spot_light_component.h"
#include <glm/glm.hpp>

glg::SpotLightComponent::SpotLightComponent(float innerCutOff, float outerCutOff) : innerCutOff(innerCutOff), outerCutOff(outerCutOff)
{
}

glg::SpotLightComponent& glg::SpotLightComponent::operator=(const SpotLightComponent& other)
{
    this->innerCutOff = other.innerCutOff;
    this->outerCutOff = other.outerCutOff;
    return *this;
}
