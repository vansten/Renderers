#include "../include/Matrix.h"

const Matrix Matrix::Identity = Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

Matrix::Matrix()
{
	_columns[0] = Vector4();
	_columns[1] = Vector4();
	_columns[2] = Vector4();
	_columns[3] = Vector4();
}

Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
	_columns[0] = Vector4(m11, m21, m31, m41);
	_columns[1] = Vector4(m12, m22, m32, m42);
	_columns[2] = Vector4(m13, m23, m33, m43);
	_columns[3] = Vector4(m14, m24, m34, m44);
}

Matrix::~Matrix()
{}

Matrix Matrix::Multiply(const Matrix& other) const
{
	Matrix otherT = Matrix::Transpose(other);
	Matrix m;
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			m._columns[i].Set(j, Vector4::Dot(_columns[i], otherT._columns[j]));
		}
	}
	return m;
}

Vector4 Matrix::MultiplyByVector3(const Vector3& v, const float w) const
{
	return MultiplyByVector4(Vector4(v, w));
}

Vector4 Matrix::MultiplyByVector4(const Vector4& v) const
{
	return Vector4(Vector4::Dot(_columns[0], v), Vector4::Dot(_columns[1], v), Vector4::Dot(_columns[2], v), Vector4::Dot(_columns[3], v));
}

std::string Matrix::ToString() const
{
	std::string s = "";

	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			s += "|" + std::to_string(_columns[j][i]) + "|";
		}
		s += "\n";
	}

	return s;
}

Vector4 Matrix::operator[](int index)
{
	if(index >= 0 && index < 4)
	{
		return _columns[index];
	}
	return Vector4();
}

Matrix Matrix::operator*(float scalar) const
{
	Matrix m = *this;
	m *= scalar;
	return m;
}

Matrix Matrix::operator/(float scalar) const
{
	Matrix m = *this;
	m *= (1.0f / scalar);
	return m;
}

void Matrix::operator*=(float scalar)
{
	_columns[0] *= scalar;
	_columns[1] *= scalar;
	_columns[2] *= scalar;
	_columns[3] *= scalar;
}

void Matrix::operator/=(float scalar)
{
	(*this) *= (1.0f / scalar);
}

Matrix Matrix::operator=(const Matrix& other)
{
	for(int i = 0; i < 4; ++i)
	{
		_columns[i] = other._columns[i];
	}

	return *this;
}

Matrix Matrix::Transpose(const Matrix& m)
{
	return Matrix(m._columns[0][0], m._columns[0][1], m._columns[0][2], m._columns[0][3],
				  m._columns[1][0], m._columns[1][1], m._columns[1][2], m._columns[1][3],
				  m._columns[2][0], m._columns[2][1], m._columns[2][2], m._columns[2][3],
				  m._columns[3][0], m._columns[3][1], m._columns[3][2], m._columns[3][3]);
}

Matrix Matrix::FromScale(const float& scale)
{
	Matrix m = Matrix::Identity * scale;
	m._columns[3].Set(3, 1.0f);
	return m;
}

Matrix Matrix::FromScale(const Vector3& scaleVector)
{
	Matrix m;
	m._columns[0].Set(0, scaleVector[0]);
	m._columns[1].Set(1, scaleVector[1]);
	m._columns[2].Set(2, scaleVector[2]);
	m._columns[3].Set(3, 1.0f);
	return m;
}

Matrix Matrix::FromTranslation(const float& x, const float& y, const float& z)
{
	Matrix m = Matrix::Identity;
	m._columns[0] = Vector4(1, 0, 0, x);
	m._columns[1] = Vector4(0, 1, 0, y);
	m._columns[2] = Vector4(0, 0, 1, z);
	m._columns[3] = Vector4(0, 0, 0, 1);
	return m;
}

Matrix Matrix::FromTranslation(const Vector3& translation)
{
	return FromTranslation(translation[0], translation[1], translation[2]);
}

Matrix Matrix::FromXYZRotation(const float& x, const float& y, const float& z)
{
	Matrix xRotation;
	Matrix yRotation;
	Matrix zRotation;

	xRotation._columns[0] = Vector4(1, 0, 0, 0);
	xRotation._columns[1] = Vector4(0, cos(x), sin(x), 0);
	xRotation._columns[2] = Vector4(0, -sin(x), cos(x), 0);
	xRotation._columns[3] = Vector4(0, 0, 0, 1);

	yRotation._columns[0] = Vector4(cos(y), 0, -sin(y), 0);
	yRotation._columns[1] = Vector4(0, 1, 0, 0);
	yRotation._columns[2] = Vector4(sin(y), 0, cos(y), 0);
	yRotation._columns[3] = Vector4(0, 0, 0, 1);

	zRotation._columns[0] = Vector4(cos(z), sin(z), 0, 0);
	zRotation._columns[1] = Vector4(-sin(z), cos(z), 0, 0);
	zRotation._columns[2] = Vector4(0, 0, 1, 0);
	zRotation._columns[3] = Vector4(0, 0, 0, 1);

	return xRotation.Multiply(yRotation).Multiply(zRotation);
}

Matrix Matrix::FromXYZRotation(const Vector3& rotation)
{
	return FromXYZRotation(rotation[0], rotation[1], rotation[2]);
}

Matrix Matrix::FromXYZRotationDegrees(const float& x, const float& y, const float& z)
{
	return FromXYZRotation(x * Deg2Rad, y * Deg2Rad, z * Deg2Rad);
}

Matrix Matrix::FromXYZRotationDegrees(const Vector3& rotation)
{
	return FromXYZRotation(rotation * Deg2Rad);
}

Matrix Matrix::LookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& upVector)
{
	Matrix M;

	Vector3 forward = targetPosition - eyePosition;
	forward.Normalize();
	Vector3 side = Vector3::Cross(upVector, forward);
	side.Normalize();
	Vector3 up = Vector3::Cross(forward, side);
	up.Normalize();

	M._columns[0] = Vector4(side[0], up[0], forward[0], -Vector3::Dot(side, eyePosition));
	M._columns[1] = Vector4(side[1], up[1], forward[1], -Vector3::Dot(up, eyePosition));
	M._columns[2] = Vector4(side[2], up[2], forward[2], -Vector3::Dot(forward, eyePosition));
	M._columns[3] = Vector4(0, 0, 0, 1);

	return M;
}

Matrix Matrix::PerspectiveLH(const float& fieldOfView, const float& aspectRatio, const float& zNearPlane, const float& zFarPlane)
{
	float w = 1.0f / (aspectRatio * tan(fieldOfView * 0.5f));
	float h = w * aspectRatio;
	float zFarDiff = zFarPlane - zNearPlane;
	float Q = zFarPlane / zFarDiff;

	return Matrix(w, 0, 0, 0,
				  0, h, 0, 0,
				  0, 0, Q, 1,
				  0, 0, -zNearPlane * Q, 0);
}

Matrix Matrix::PerspectiveLHDeg(const float& fieldOfView, const float& aspectRatio, const float& zNearPlane, const float& zFarPlane)
{
	return PerspectiveLH(fieldOfView * Deg2Rad, aspectRatio, zNearPlane, zFarPlane);
}
