#pragma once

#include <vector>

#include "Vertex.h"
#include "Int3.h"
#include "Matrix.h"

#include "DirectionalLight.h"

class DeviceContext;

class Shape
{
protected:
	std::vector<VertexPositionUVNormal> _vertices;
	std::vector<Int3> _indices;
	Matrix _modelMatrix;
	Vector3 _translation;
	Vector3 _rotation;
	Vector3 _scale;

public:
	Shape();
	virtual ~Shape();

	void Translate(const float& x, const float& y, const float& z);
	void Rotate(const float& x, const float& y, const float& z);
	void Scale(const float& x, const float& y, const float& z);
	void CalculateModelMatrix();
	
	virtual void Draw(DeviceContext* deviceContext, DirectionalLight* light);
};