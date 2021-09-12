#include "camera_component.h"

glg::CameraComponent::CameraComponent(float fov, float nearPlane, float farPlane) : fov(fov), nearPlane(nearPlane), farPlane(farPlane)
{
}

glg::CameraComponent& glg::CameraComponent::operator=(const glg::CameraComponent& other)
{
	fov = other.fov;
	nearPlane = other.nearPlane;
	farPlane = other.farPlane;
	return *this;
}
