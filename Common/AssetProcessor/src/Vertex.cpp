#include "../include/Vertex.h"

VertexPositionColor::VertexPositionColor() : Position(), Color() {}

VertexPositionColor::VertexPositionColor(Vector3 position, Color32 color) : Position(position), Color(color) {}

VertexPositionColor::VertexPositionColor(const VertexPositionColor& vpc) : Position(vpc.Position), Color(vpc.Color) {}

VertexPositionColor::~VertexPositionColor() {}

VertexPositionUVNormal::VertexPositionUVNormal() : Position(), Normal(), UV()
{}

VertexPositionUVNormal::VertexPositionUVNormal(const Vector3& position, const Vector3& normal, const Vector2& uv) : Position(position), Normal(normal), UV(uv)
{}

VertexPositionUVNormal::~VertexPositionUVNormal()
{}
