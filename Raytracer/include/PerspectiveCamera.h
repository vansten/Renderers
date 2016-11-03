#pragma once

#include "Camera.h"
#include "Matrix.h"

namespace raytracer
{
	class PerspectiveCamera : public Camera
	{
	protected:
		Matrix _rotationMatrix;
		float _fov;
		float _tanFovDiv2;
		float _near;
		float _far;

	public:
		PerspectiveCamera(float fov, float nearPlane, float farPlane, Vector3 position = Vector3::Zero, Matrix rotation = Matrix::Identity);
		PerspectiveCamera(const PerspectiveCamera& other);
		
		virtual void ConstructRay(Ray& r, const Pixel& p) const override;
	};
}