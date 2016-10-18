#pragma once

#include "Vector.h"
#include "Types.h"
#include "Matrix.h"
#include "Vertex.h"
#include "Texture.h"
#include "DirectionalLight.h"

class Buffer;
class DepthBuffer;
class Triangle;

class Rasterizer
{
public:
	Rasterizer();
	~Rasterizer();

	void DrawTriangleWithTexture(Buffer* buffer, DepthBuffer* depthBuffer, const Matrix& objectToWorld, const Matrix& mvp, const VertexPositionUVNormal& v1, const VertexPositionUVNormal& v2, const VertexPositionUVNormal& v3, const Texture* texture, const DirectionalLight* directionalLight) const;
};
