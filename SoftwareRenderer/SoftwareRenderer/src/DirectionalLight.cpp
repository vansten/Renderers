#include "../include/DirectionalLight.h"

DirectionalLight::DirectionalLight(Vector3 direction, Color32 color) : Direction(direction), Color(color)
{
	Direction.Normalize();
}

DirectionalLight::~DirectionalLight()
{}
