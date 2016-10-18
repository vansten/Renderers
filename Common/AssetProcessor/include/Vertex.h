#pragma once

#include "Vector.h"
#include "Types.h"

class VertexPositionColor
{
public:
	Vector3 Position;
	Color32 Color;

public:
	VertexPositionColor();
	VertexPositionColor(Vector3 position, Color32 color);
	VertexPositionColor(const VertexPositionColor& vpc);
	~VertexPositionColor();
};

class VertexPositionUVNormal
{
public:
	Vector3 Position;
	Vector3 Normal;
	Vector2 UV;

public:
	VertexPositionUVNormal();
	VertexPositionUVNormal(const Vector3& position, const Vector3& normal, const Vector2& uv);
	~VertexPositionUVNormal();
};