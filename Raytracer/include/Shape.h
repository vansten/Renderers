#pragma once

#include "Ray.h"
#include "RaycastHit.h"

class Shape
{
	friend class Ray;
public:
	Color24 Color;

public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual std::string ToString() const = 0;
	virtual Color24 GetColor() const;

protected:
	virtual bool Intersects(const Ray& r, RaycastHit& hit) const = 0;
};

