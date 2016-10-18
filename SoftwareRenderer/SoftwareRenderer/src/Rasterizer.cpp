#include "../include/Rasterizer.h"

#include "Buffer.h"
#include "DepthBuffer.h"

#include <cmath>

#define VERTEX_LIGHTING 0
#if VERTEX_LIGHTING
#define PIXEL_LIGHTING 0
#else
#define PIXEL_LIGHTING 1
#endif

float Min(const float a, const float b, const float c)
{
	if(a <= b)
	{
		return a <= c ? a : c;
	}
	else
	{
		return b <= c ? b : c;
	}
}

float Max(const float a, const float b, const float c)
{
	if(a >= b)
	{
		return a >= c ? a : c;
	}
	else
	{
		return b >= c ? b : c;
	}
}

Rasterizer::Rasterizer()
{}


Rasterizer::~Rasterizer()
{}

void Rasterizer::DrawTriangleWithTexture(Buffer* buffer, DepthBuffer* depthBuffer, const Matrix& objectToWorld, const Matrix& mvp, const VertexPositionUVNormal& v1, const VertexPositionUVNormal& v2, const VertexPositionUVNormal& v3, const Texture* texture, const DirectionalLight* directionalLight) const
{
	const Vector4& halfDimMult = Vector4(buffer->GetWidth() * 0.5f, buffer->GetHeight() * 0.5f, 1.0f, 1.0f);
	const Vector4& halfDimAdd = Vector4(buffer->GetWidth() * 0.5f, buffer->GetHeight() * 0.5f, 0.0f, 0.0f);
	Vector4& v1Screen = mvp.MultiplyByVector3(v1.Position);
	Vector4& v2Screen = mvp.MultiplyByVector3(v2.Position);
	Vector4& v3Screen = mvp.MultiplyByVector3(v3.Position);
	v1Screen /= v1Screen[3];
	v2Screen /= v2Screen[3];
	v3Screen /= v3Screen[3];

	const Vector4& v2v1 = v2Screen - v1Screen;
	const Vector4& v3v1 = v3Screen - v1Screen;
	const Vector4& cross = Vector4::Cross(v3v1, v2v1);
	if(cross[2] < 0.0f)
	{
		return;
	}

	v1Screen = v1Screen * halfDimMult + halfDimAdd;
	v2Screen = v2Screen * halfDimMult + halfDimAdd;
	v3Screen = v3Screen * halfDimMult + halfDimAdd;

	int minX = (int)clamp(Min(v1Screen[0], v2Screen[0], v3Screen[0]), 0, halfDimMult[0] * 2.0f);
	int maxX = (int)(ceil(clamp(Max(v1Screen[0], v2Screen[0], v3Screen[0]), 0, halfDimMult[0] * 2.0f)));
	int minY = (int)clamp(Min(v1Screen[1], v2Screen[1], v3Screen[1]), 0, halfDimMult[1] * 2.0f);
	int maxY = (int)(ceil(clamp(Max(v1Screen[1], v2Screen[1], v3Screen[1]), 0, halfDimMult[1] * 2.0f)));

	float dx12 = v1Screen[0] - v2Screen[0];
	float dx23 = v2Screen[0] - v3Screen[0];
	float dx31 = v3Screen[0] - v1Screen[0];
	float dy12 = v1Screen[1] - v2Screen[1];
	float dy23 = v2Screen[1] - v3Screen[1];
	float dy31 = v3Screen[1] - v1Screen[1];

	float lambda1den = 1.0f / ((dy23 * -dx31) + (dx23 * dy31));
	float lambda2den = 1.0f / ((dy31 * dx23) + (-dx31 * dy23));

	bool tl1 = (dy12 < 0.0f || (dy12 == 0 && dx12 > 0));
	bool tl2 = (dy23 < 0.0f || (dy23 == 0 && dx23 > 0));
	bool tl3 = (dy31 < 0.0f || (dy31 == 0 && dx31 > 0));

	float dxx3 = 0.0f;
	float dyy3 = 0.0f;
	float lambda1 = 0.0f;
	float lambda2 = 0.0f;
	float lambda3 = 0.0f;
	Vector2 finalUV;
	float finalZ;

	Vector3 directionInverted = -(directionalLight->Direction);
	Color32 finalColor;
#if VERTEX_LIGHTING
	Color32 vertex1Light = Color32::White;
	Color32 vertex2Light = Color32::White;
	Color32 vertex3Light = Color32::White;
	if(directionalLight != nullptr)
	{
		Vector4 v1Normal = objectToWorld.MultiplyByVector4(Vector4(v1.Normal, 0));
		Vector4 v2Normal = objectToWorld.MultiplyByVector4(Vector4(v2.Normal, 0));
		Vector4 v3Normal = objectToWorld.MultiplyByVector4(Vector4(v3.Normal, 0));
		Vector3 v1NormalWorld(v1Normal[0], v1Normal[1], v1Normal[2]);
		Vector3 v2NormalWorld(v2Normal[0], v2Normal[1], v2Normal[2]);
		Vector3 v3NormalWorld(v3Normal[0], v3Normal[1], v3Normal[2]);
		float dot = clamp(Vector3::Dot(directionInverted, v1NormalWorld));
		vertex1Light = directionalLight->Color * dot;
		dot = clamp(Vector3::Dot(directionInverted, v2NormalWorld));
		vertex2Light = directionalLight->Color * dot;
		dot = clamp(Vector3::Dot(directionInverted, v3NormalWorld));
		vertex3Light = directionalLight->Color * dot;
	}
#endif

	for(int x = minX; x < maxX; ++x)
	{
		for(int y = minY; y < maxY; ++y)
		{
			dxx3 = x - v3Screen[0];
			dyy3 = y - v3Screen[1];
			float a = dx12 * (y - v1Screen[1]) - dy12 * (x - v1Screen[0]);
			float b = dx23 * (y - v2Screen[1]) - dy23 * (x - v2Screen[0]);
			float c = dx31 * dyy3 - dy31 * dxx3;
			if(
				(a > 0.0f || (tl1 && a >= 0.0f)) &&
				(b > 0.0f || (tl2 && b >= 0.0f)) &&
				(c > 0.0f || (tl3 && c >= 0.0f))
				)
			{
				lambda1 = (dy23 * dxx3 + -dx23 * dyy3) * lambda1den;
				lambda2 = (dy31 * dxx3 + -dx31 * dyy3) * lambda2den;
				lambda3 = 1.0f - lambda1 - lambda2;
				finalZ = lambda1 * v1.Position[2] + lambda2 * v2.Position[2] + lambda3 * v3.Position[2];
				if(depthBuffer->GetDepth(x, y) > finalZ)
				{
					finalUV = v1.UV * lambda1 + v2.UV * lambda2 + v3.UV * lambda3;
					depthBuffer->SetDepth(x, y, finalZ);
					Color32 c = texture->GetPixel(finalUV[0], finalUV[1]);
#if VERTEX_LIGHTING
					finalColor = vertex1Light * lambda1 + vertex2Light * lambda2 + vertex3Light * lambda3;
					buffer->SetPixel(x, y, c * finalColor);
#elif PIXEL_LIGHTING
					Vector4 pixelNormal = objectToWorld.MultiplyByVector3(v1.Normal) * lambda1 + objectToWorld.MultiplyByVector3(v2.Normal) * lambda2 + objectToWorld.MultiplyByVector3(v3.Normal) * lambda3;
					pixelNormal.Normalize();
					float dot = clamp(Vector3::Dot(Vector3(pixelNormal[0], pixelNormal[1], pixelNormal[2]), directionInverted));
					finalColor = directionalLight->Color * dot;
					buffer->SetPixel(x, y, c * finalColor);
#endif
				}
			}
		}
	}
}