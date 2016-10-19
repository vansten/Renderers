#pragma once
#include <cmath>
#include <string>

#include "Types.h"

#define USE_SIMD 1

const float PI = 3.14159265359f;
const float TwoPI = 2.0f * PI;
const float PiOver2 = PI * 0.5f;
const float PiOver4 = PI * 0.25f;
const float Deg2Rad = PI / 180.0f;
const float Rad2Deg = 180.0f / PI;
const float epsilon = 0.00001f;

template<int N>
class Vector
{
public:
#pragma region Statics

	static Vector<N> Zero;
	static Vector<N> One;

#pragma endregion

public:
#pragma region Variables
	union
	{
		float _components[N];
		__m128 _simdValue;
	};
#pragma endregion

public:
#pragma region Constructors and destructors
	Vector(const float components[N]) : _simdValue()
	{
		for(int i = 0; i < N; ++i)
		{
			_components[i] = components[i];
		}
	}

	Vector(const Vector<N - 1>& v, float f) : _simdValue()
	{
		for(int i = 0; i < N - 1; ++i)
		{
			_components[i] = v[i];
		}
		_components[N - 1] = f;
	}

	Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) : _simdValue()
	{
		float c[] = {x, y, z, w};
		for(int i = 0; i < N; ++i)
		{
			_components[i] = c[i];
		}
	}

	virtual ~Vector()
	{

	}

#pragma endregion

#pragma region Vector operations (Normalize, Length, LengthSquared, ToString)

	Vector<N> Normalized() const
	{
		Vector<N> vec(*this);
		vec.Normalize();
		return vec;
	}

	void Normalize()
	{
		float l = Length();
		if(l > epsilon)
		{
#if USE_SIMD
			float onePerL = 1.0f / l;
			__m128 len = _mm_set_ps(onePerL, onePerL, onePerL, onePerL);
			_simdValue = _mm_mul_ps(_simdValue, len);
#else
			for(int i = 0; i < N; ++i)
			{
				_components[i] /= l;
			}
#endif
		}
		else
		{
			*this = Vector<N>::Zero;
		}
	}

	float Length() const
	{
#if USE_SIMD
		__m128 l = _mm_mul_ps(_simdValue, _simdValue);
		return sqrt(l.m128_f32[0] + l.m128_f32[1] + l.m128_f32[2] + l.m128_f32[3]);
#else
		float l = 0.0f;
		for(int i = 0; i < N; ++i)
		{
			l += (_components[i] * _components[i]);
		}
		return sqrt(l);
#endif
	}

	float LengthSquared() const
	{
#if USE_SIMD
		__m128 l = _simdValue * _simdValue;
		return sqrt(l.m128_f32[0] + l.m128_f32[1] + l.m128_f32[2] + l.m128_f32[3]);
#else
		float l = 0.0f;
		for(int i = 0; i < N; ++i)
		{
			l += (_components[i] * _components[i]);
		}
		return l;
#endif
	}

	std::string ToString() const
	{
		std::string s = "[";
		for(int i = 0; i < N; ++i)
		{
			s += std::to_string(_components[i]);
			if(i < (N - 1))
			{
				s += ", ";
			}
		}
		s += "]";
		return s;
	}

#pragma endregion

#pragma region Operators

	float& operator[](uint32 index) //Reference for writing too
	{
		return _components[index];
	}

	float operator[](uint32 index) const //Non-reference read-only
	{
		return _components[index];
	}

	void operator=(const Vector<N>& other)
	{
#if USE_SIMD
		_simdValue = __m128(other._simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			_components[i] = other._components[i];
		}
#endif
	}

	Vector<N> operator-() const
	{
		static Vector<N> vec;
#if USE_SIMD
		static __m128 minusOne = _mm_set_ps(-1.0f, -1.0f, -1.0f, -1.0f);
		vec._simdValue = _mm_mul_ps(_simdValue, minusOne);
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = -_components[i];
		}
#endif
		return vec;
	}

	Vector<N> operator+()
	{
		static Vector<N> vec;
#if USE_SIMD
		vec._simdValue = _simdValue;
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = _components[i];
		}
#endif
		return vec;
	}

	Vector<N> operator-(Vector<N> other) const
	{
		static Vector<N> vec;
#if USE_SIMD
		vec._simdValue = _mm_sub_ps(_simdValue, other._simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = _components[i] - other._components[i];
		}
#endif
		return vec;
	}

	Vector<N> operator+(Vector<N> other) const
	{
		static Vector<N> vec;
#if USE_SIMD
		vec._simdValue = _mm_add_ps(_simdValue, other._simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = _components[i] + other._components[i];
		}
#endif
		return vec;
	}

	Vector<N> operator*(const Vector<N>& other) const
	{
		static Vector<N> vec;
#if USE_SIMD
		vec._simdValue = _mm_mul_ps(_simdValue, other._simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = _components[i] * other._components[i];
		}
#endif
		return vec;
	}

	Vector<N> operator*(float s) const
	{
		static Vector<N> vec;
#if USE_SIMD
		__m128 sSimd = _mm_set_ps(s, s, s, s);
		vec._simdValue = _mm_mul_ps(sSimd, _simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = _components[i] * s;
		}
#endif
		return vec;
	}

	Vector<N> operator/(float s) const
	{
		static Vector<N> vec;
#if USE_SIMD
		__m128 sSimd = _mm_set_ps(s, s, s, s);
		vec._simdValue = _mm_div_ps(_simdValue, sSimd);
#else
		for(int i = 0; i < N; ++i)
		{
			vec._components[i] = _components[i] / s;
		}
#endif
		return vec;
	}

	void operator+=(const Vector<N>& other)
	{
#if USE_SIMD
		_simdValue = _mm_add_ps(_simdValue, other._simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			_components[i] += other._components[i];
		}
#endif
	}

	void operator-=(const Vector<N>& other)
	{
#if USE_SIMD
		_simdValue = _mm_sub_ps(_simdValue, other._simdValue);
#else
		for(int i = 0; i < N; ++i)
		{
			_components[i] -= other._components[i];
		}
#endif
	}

	void operator*=(float s)
	{
#if USE_SIMD
		__m128 sSimd = _mm_set_ps(s, s, s, s);
		_simdValue = _mm_mul_ps(_simdValue, sSimd);
#else
		for(int i = 0; i < N; ++i)
		{
			_components[i] *= s;
		}
#endif
	}

	void operator/=(float s)
	{
		float onePerS = 1.0f / s;
		return (*this) *= onePerS;
	}

#pragma endregion

#pragma region Static operations (Cross, Dot, Clamp, Lerp, Angle)

	static Vector<N> Clamp(const Vector<N>& value, const Vector<N>& min = Vector<N>::Zero, const Vector<N>& max = Vector<N>::One)
	{
		static Vector<N> v;
		for(int i = 0; i < N; ++i)
		{
			v._components[i] = fmin(fmax(min[i], value[i]), max[i]);
		}
		return v;
	}

	static Vector<N> Cross(const Vector<N>& from, const Vector<N>& to = One)
	{
		if(N > 2)
		{
			//Cross product
			Vector<N> vec;
#if USE_SIMD
			//SIMD version of cross product
			vec._simdValue = _mm_sub_ps(
				_mm_mul_ps(_mm_shuffle_ps(from._simdValue, from._simdValue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(to._simdValue, to._simdValue, _MM_SHUFFLE(3, 1, 0, 2))),
				_mm_mul_ps(_mm_shuffle_ps(from._simdValue, from._simdValue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(to._simdValue, to._simdValue, _MM_SHUFFLE(3, 0, 2, 1)))
			);
#else
			//| a.x |   | b.x |   | a.y * b.z - a.z * b.y |
			//| a.y | X | b.y | = | a.z * b.x - a.x * b.z |
			//| a.z |   | b.z |   | a.x * b.y - a.y * b.x |

			vec._components[0] = from[1] * to[2] - from[2] * to[1];
			vec._components[1] = from[2] * to[0] - from[0] * to[2];
			vec._components[2] = from[0] * to[1] - from[1] * to[0];
			vec._components[3] = from[3];
#endif
			return vec;

		}
		else if(N > 1) //Make sure that we'll not computing Vector<1>::Cross or Vector<0>::Cross
		{
			return Vector<N>(from._components[1], -from._components[0]);
		}
		return Vector<N>();
	}

	static Vector<N> Lerp(const Vector<N>& from, const Vector<N>& to, float t)
	{
		static Vector<N> v;
#if USE_SIMD
		__m128 tSimd = _mm_set_ps(t, t, t, t);
		__m128 oneMinusTSimd = _mm_set_ps(1.0f - t, 1.0f - t, 1.0f - t, 1.0f - t);
		v._simdValue = _mm_add_ps(_mm_mul_ps(from._simdValue, oneMinusTSimd), _mm_mul_ps(to._simdValue, tSimd));
#else
		for(int i = 0; i < N; ++i)
		{
			v._components[i] = MMath::Lerp(from._components[i], to._components[i], t);
		}
#endif
		return v;
	}

	static Vector<N> Slerp(const Vector<N>& from, const Vector<N>& to, float t)
	{
		float dot = Dot(from.Normalized(), to.Normalized());
		float omega = acos(dot);
		float sinOmega = sin(omega);
		return from * sin((1.0f - t) * omega) / sinOmega + to * sin(t * omega) / sinOmega;
	}

	static float Dot(const Vector<N>& v1, const Vector<N>& v2)
	{
		float dot = 0.0f;
#if USE_SIMD
		__m128 mulProduct = _mm_mul_ps(v1._simdValue, v2._simdValue);
		dot = mulProduct.m128_f32[0] + mulProduct.m128_f32[1] + mulProduct.m128_f32[2] + mulProduct.m128_f32[3];
#else
		for(int i = 0; i < N; ++i)
		{
			dot += v1._components[i] * v2._components[i];
		}
#endif
		return dot;
	}

	static float Angle(const Vector<N>& from, const Vector<N>& to)
	{
		return acos(Dot(from, to));
	}

	static float AngleD(const Vector<N>& from, const Vector<N>& to)
	{
		return acos(Dot(from, to)) * Rad2Deg;
	}

#pragma endregion
};

template<int N>
Vector<N> Vector<N>::Zero = Vector<N>();
template<int N>
Vector<N> Vector<N>::One = Vector<N>(1.0f, 1.0f, 1.0f, 1.0f);

typedef Vector<2> Vector2;
typedef Vector<3> Vector3;
typedef Vector<4> Vector4;

Vector3 Reflect(const Vector3& v, const Vector3& normal);