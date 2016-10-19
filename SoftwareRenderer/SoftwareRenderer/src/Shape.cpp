#include "../include/Shape.h"

#include "../include/DeviceContext.h"

Shape::Shape()
{
	_translation = Vector3::Zero;
	_rotation = Vector3::Zero;
	_scale = Vector3::One;
	CalculateModelMatrix();
}

Shape::~Shape()
{
}

void Shape::Translate(const float& x, const float& y, const float& z)
{
	_translation += Vector3(x, y, z);
	CalculateModelMatrix();
}

void Shape::Rotate(const float& x, const float& y, const float& z)
{
	_rotation += Vector3(x, y, z);
	CalculateModelMatrix();
}

void Shape::Scale(const float& x, const float& y, const float& z)
{
	_scale = Vector3(x, y, z);
	CalculateModelMatrix();
}

void Shape::CalculateModelMatrix()
{
	Matrix T = Matrix::FromTranslation(_translation);
	Matrix R = Matrix::FromXYZRotation(_rotation * Deg2Rad);
	Matrix S = Matrix::FromScale(_scale);
	_modelMatrix = T.Multiply(R).Multiply(S);
}

void Shape::Draw(DeviceContext* deviceContext, const DirectionalLight* light, const SpotLight* spotlight)
{
	deviceContext->DrawIndexed(_modelMatrix, _vertices, _indices, light, spotlight);
}
