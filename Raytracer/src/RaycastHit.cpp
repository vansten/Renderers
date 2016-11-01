#include "../include/RaycastHit.h"

namespace raytracer
{
	IntersectionPoint::IntersectionPoint(Vector3 point, Vector3 normal) : Point(point), Normal(normal)
	{

	}

	RaycastHit::RaycastHit()
	{
		_intersectionPoints.reserve(2);
	}

	RaycastHit::~RaycastHit()
	{
		_intersectionPoints.clear();
	}

	void RaycastHit::AddIntersectionPoint(const IntersectionPoint& p)
	{
		_intersectionPoints.push_back(p);
	}

	const std::vector<IntersectionPoint>& RaycastHit::GetIntersectionPoints()
	{
		return _intersectionPoints;
	}

	void RaycastHit::Clear()
	{
		_intersectionPoints.clear();
	}
}