#include "..\include\PerspectiveCamera.h"

namespace raytracer
{
	PerspectiveCamera::PerspectiveCamera(float fov, float nearPlane, float farPlane) : Camera(), _fov(fov), _near(nearPlane), _far(farPlane)
	{
		_tanFovDiv2 = tan(fov * Deg2Rad * 0.5);
	}

	PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& other) : Camera(other), _fov(other._fov), _near(other._near), _far(other._far), _tanFovDiv2(other._tanFovDiv2)
	{

	}

	void PerspectiveCamera::ConstructRay(Ray& r, const Pixel& p) const
	{
		r.Origin[0] = 0.0f;
		r.Origin[1] = 0.0f;
		r.Origin[2] = -10.0f;
		r.Direction[0] = p.X * _tanFovDiv2;
		r.Direction[1] = p.Y * _tanFovDiv2 * _onePerAspectRatio;
		r.Direction[2] = 1.0f;
		//return Ray(0.0f, 0.0f, -10.0f, p.X * _tanFovDiv2 * _aspectRatio, p.Y * _tanFovDiv2, 1.0f);
	}
}