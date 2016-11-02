#include "../include/Camera.h"
#include "../include/Engine.h"

namespace raytracer
{
	Camera::Camera(Vector3 position) : _position(position)
	{
		_aspectRatio = (float)Engine::GetInstance()->GetWidth() / (float)Engine::GetInstance()->GetHeight();
		_onePerAspectRatio = 1.0f / _aspectRatio;
	}

	Camera::Camera(const Camera& other) : _aspectRatio(other._aspectRatio), _onePerAspectRatio(other._onePerAspectRatio), _position(other._position)
	{

	}

	Camera::~Camera()
	{

	}
}