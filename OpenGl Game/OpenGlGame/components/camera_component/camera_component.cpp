#include "camera_component.h"

CameraComponent::CameraComponent(float fov, float nearPlane, float farPlane) : fov(fov), nearPlane(nearPlane), farPlane(farPlane)
{
}

CameraComponent& CameraComponent::operator=(const CameraComponent& other)
{
	fov = other.fov;
	nearPlane = other.nearPlane;
	farPlane = other.farPlane;
	return *this;
}
