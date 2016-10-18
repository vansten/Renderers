#pragma once

#include "Vector.h"

//Only 4x4 matrices
class Matrix
{
public:
	static const Matrix Identity;

protected:
	Vector4 _columns[4];

public:
	Matrix();
	Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
	~Matrix();

	Matrix Multiply(const Matrix& other) const;
	Vector4 MultiplyByVector3(const Vector3& v) const;
	Vector4 MultiplyByVector4(const Vector4& v) const;
	std::string ToString() const;

	Vector4 operator[](int index);
	Matrix operator*(float scalar) const;
	Matrix operator/(float scalar) const;
	void operator*=(float scalar);
	void operator/=(float scalar);
	Matrix operator=(const Matrix& other);
	
	static Matrix Transpose(const Matrix& m);
	static Matrix FromScale(const float& scale);
	static Matrix FromScale(const Vector3& scaleVector);
	static Matrix FromTranslation(const float& x, const float& y, const float& z);
	static Matrix FromTranslation(const Vector3& translation);
	static Matrix FromXYZRotation(const float& x, const float& y, const float& z);
	static Matrix FromXYZRotation(const Vector3& rotation);
	static Matrix FromXYZRotationDegrees(const float& x, const float& y, const float& z);
	static Matrix FromXYZRotationDegrees(const Vector3& rotation);
	static Matrix LookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& upVector);
	static Matrix PerspectiveLH(const float& fieldOfView, const float& aspectRatio, const float& zNearPlane, const float& zFarPlane);
	static Matrix PerspectiveLHDeg(const float& fieldOfView, const float& aspectRatio, const float& zNearPlane, const float& zFarPlane);
};

