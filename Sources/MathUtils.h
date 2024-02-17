#pragma once

#include <cmath>
#include <DirectXMath.h>
namespace sr
{
	struct Vector2		{ float x, y; };
	struct Vector3		{ float x, y, z; };
	struct Vector4		{ float x, y, z, w; };
	struct Quaternion	{ float x, y, z, w; };
	struct Matrix3x3	{ float m[3][3]; };
	struct Matrix4x4	{ float m[4][4]; };

	namespace math
	{
		Vector4 Vector4Dot(const Vector4& v1, const Vector4& v2);
		Vector4 Vector4Multiplay(const Vector4& v, const Matrix4x4& m);
		Vector4 Vector4Normalize(const Vector4& v);
	}
}

sr::Vector4 sr::math::Vector4Dot(const Vector4& v1, const Vector4& v2)
{
	const float result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	return Vector4{ result ,result ,result ,result };
}

sr::Vector4 sr::math::Vector4Multiplay(const Vector4& v, const Matrix4x4& m)
{
	return Vector4{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
		v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3] };
}

sr::Vector4 sr::math::Vector4Normalize(const Vector4& v)
{
	const float length_squared = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	float inv_length = 0.0f;

	// Prevent divide by zero
	if (length_squared > 0.0f)
	{
		inv_length = 1.0f / std::sqrt(length_squared);
	}

	return Vector4{ v.x * inv_length, v.y * inv_length, v.z * inv_length, v.w * inv_length };
}
