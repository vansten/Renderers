#pragma once
#include "Shape.h"
#include "Vector.h"

class Plane : public Shape
{
protected:
	//Ax + By + Cz + D = 0
	Vector3 _planePoint; //x,y,z
	Vector3 _normal; //A, B, C
	float _d; //-D

public:

	Plane(Color24 color = Color24::Magenta);
	Plane(float x0, float y0, float z0, float nx, float ny, float nz, Color24 color = Color24::Magenta);
	Plane(Vector3 point, Vector3 normal, Color24 color = Color24::Magenta);
	~Plane();

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual std::string ToString() const override;

	const Vector3& GetNormal() const;
	const Vector3& GetPoint() const;

protected:
	virtual bool Intersects(const Ray& r, RaycastHit& hit) const override;
};

