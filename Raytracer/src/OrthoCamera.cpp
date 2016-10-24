#include "../include/OrthoCamera.h"
#include "../include/Engine.h"

namespace raytracer
{
	OrthoCamera::OrthoCamera(float size, float cameraZ) : _size(size), _cameraZ(cameraZ), Camera()
	{
	}

	OrthoCamera::OrthoCamera(const OrthoCamera& other) : Camera(other), _size(other._size), _cameraZ(other._cameraZ)
	{

	}

	void OrthoCamera::ConstructRay(Ray& r, const Pixel& p) const
	{
		r.Origin[0] = p.X * _size;
		r.Origin[1] = p.Y * _size * _onePerAspectRatio;
		r.Origin[2] = _cameraZ;
		r.Direction[0] = 0.0f;
		r.Direction[1] = 0.0f;
		r.Direction[2] = 1.0f;
		//return Ray(p.X * _size, p.Y * _size * _onePerAspectRatio, _cameraZ, 0.0f, 0.0f, 1.0f);
	}
}