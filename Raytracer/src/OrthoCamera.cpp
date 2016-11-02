#include "../include/OrthoCamera.h"
#include "../include/Engine.h"

namespace raytracer
{
	OrthoCamera::OrthoCamera(float size, Vector3 position) : _size(size), Camera(position)
	{
	}

	OrthoCamera::OrthoCamera(const OrthoCamera& other) : Camera(other), _size(other._size)
	{

	}

	void OrthoCamera::ConstructRay(Ray& r, const Pixel& p) const
	{
		r.Origin[0] = p.X * _size + _position[0];
		r.Origin[1] = p.Y * _size * _onePerAspectRatio + _position[1];
		r.Origin[2] = _position[2];
		r.Direction[0] = 0.0f;
		r.Direction[1] = 0.0f;
		r.Direction[2] = 1.0f;
		//return Ray(p.X * _size, p.Y * _size * _onePerAspectRatio, _cameraZ, 0.0f, 0.0f, 1.0f);
	}
}