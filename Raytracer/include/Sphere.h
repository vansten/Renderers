#pragma once
#include "Shape.h"

class Sphere : public Shape
{
public:
	Vector3 Center;
	float Radius;

public:
	Sphere();
	Sphere(Vector3 center, float radius);
	Sphere(float x0, float y0, float z0, float radius);
	virtual ~Sphere();

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual std::string ToString() const override;

protected:
	virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
};

