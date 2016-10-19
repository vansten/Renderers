#include "../include/Vector.h"

Vector3 Reflect(const Vector3& v, const Vector3& normal)
{
	return v - (normal * 2.0f * Vector3::Dot(v, normal));
}
