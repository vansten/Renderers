#include "../include/RaycastHit.h"

RaycastHit::RaycastHit()
{
	_intersectionPoints.reserve(2);
}

RaycastHit::~RaycastHit()
{
	_intersectionPoints.clear();
}

void RaycastHit::AddIntersectionPoint(const Vector3& p)
{
	_intersectionPoints.push_back(p);
}

const std::vector<Vector3>& RaycastHit::GetIntersectionPoints()
{
	return _intersectionPoints;
}

void RaycastHit::Clear()
{
	_intersectionPoints.clear();
}
