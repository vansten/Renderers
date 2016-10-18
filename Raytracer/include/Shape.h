#pragma once

#include "Ray.h"
#include "RaycastHit.h"

class Shape
{
	friend class Ray;

public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual std::string ToString() const = 0;

protected:
	virtual bool Intersects(const Ray& r, RaycastHit& hit) const = 0;
};

