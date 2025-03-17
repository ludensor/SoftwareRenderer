#pragma once

namespace math
{
	constexpr float PI = 3.1415926535897932f;

	struct Vector2 { float x, y; };
	struct Vector3 { float x, y, z; };
	struct Vector4 { float x, y, z, w; };
	struct Quaternion { float x, y, z, w; };
	struct Matrix4x4 { float m[4][4]; };

	constexpr Vector2 VECTOR2_ZERO{ 0.0f, 0.0f };
	constexpr Vector2 VECTOR2_ONE{ 1.0f, 1.0f };
	constexpr Vector2 VECTOR2_RIGHT{ 1.0f, 0.0f };
	constexpr Vector2 VECTOR2_LEFT{ -1.0f, 0.0f };
	constexpr Vector2 VECTOR2_UP{ 0.0f, 1.0f };
	constexpr Vector2 VECTOR2_DOWN{ 0.0f, -1.0f };

	constexpr Vector3 VECTOR3_ZERO{ 0.0f, 0.0f, 0.0f };
	constexpr Vector3 VECTOR3_ONE{ 1.0f, 1.0f, 1.0f };
	constexpr Vector3 VECTOR3_FORWARD{ 1.0f, 0.0f, 0.0f };
	constexpr Vector3 VECTOR3_BACKWARD{ -1.0f, 0.0f, 0.0f };
	constexpr Vector3 VECTOR3_RIGHT{ 0.0f, 1.0f, 0.0f };
	constexpr Vector3 VECTOR3_LEFT{ 0.0f, -1.0f, 0.0f };
	constexpr Vector3 VECTOR3_UP{ 0.0f, 0.0f, 1.0f };
	constexpr Vector3 VECTOR3_DOWN{ 0.0f, 0.0f, -1.0f };

	constexpr Vector4 VECTOR4_ZERO{ 0.0f, 0.0f, 0.0f, 0.0f };
	constexpr Vector4 VECTOR4_ONE{ 1.0f, 1.0f, 1.0f, 1.0f };

	constexpr Quaternion QUATERNION_IDENTITY{ 0.0f, 0.0f, 0.0f, 1.0f };

	constexpr Matrix4x4 MATRIX_IDENTITY{ 1.0f, 0.0f, 0.0f, 0.0f,
										 0.0f, 1.0f, 0.0f, 0.0f,
										 0.0f, 0.0f, 1.0f, 0.0f,
										 0.0f, 0.0f, 0.0f, 1.0f };

	/* General functions */
	inline int32_t CeilToInt(float x);
	inline int32_t FloorToInt(float x);
	inline int32_t Clamp(int32_t x, int32_t min, int32_t max);
	inline float Clamp(float x, float min, float max);
	inline int32_t Min(int32_t a, int32_t b);
	inline float Min(float a, float b);
	inline int32_t Max(int32_t a, int32_t b);
	inline float Max(float a, float b);
	inline float DegreesToRadians(float degrees);
	inline float RadiansToDegrees(float radians);

	/* float related functions */
	inline float FloatLerp(float a, float b, float t);
	inline float FloatSaturate(float f);
	inline float FloatFromUChar(uint8_t value);
	inline uint8_t FloatToUChar(float value);

	/* Vector2 related functions */
	inline const Vector2 operator+(const Vector2& v);
	inline const Vector2 operator-(const Vector2& v);
	inline const Vector2 operator+(const Vector2& a, const Vector2& b);
	inline const Vector2 operator-(const Vector2& a, const Vector2& b);
	inline const Vector2 operator*(const Vector2& v, float s);
	inline const Vector2 operator*(float s, const Vector2& v);
	inline const Vector2 operator/(const Vector2& v, float s);
	inline Vector2& operator+=(Vector2& a, const Vector2& b);
	inline Vector2& operator-=(Vector2& a, const Vector2& b);
	inline Vector2& operator*=(Vector2& v, float s);
	inline Vector2& operator/=(Vector2& v, float s);
	inline float Vector2Dot(const Vector2& a, const Vector2& b);
	inline const Vector2 Vector2Lerp(const Vector2& a, const Vector2& b, float t);
	inline const Vector2 Vector2Min(const Vector2& a, const Vector2& b);
	inline const Vector2 Vector2Max(const Vector2& a, const Vector2& b);

	/* Vector3 related functions */
	inline const Vector3 operator+(const Vector3& v);
	inline const Vector3 operator-(const Vector3& v);
	inline const Vector3 operator+(const Vector3& a, const Vector3& b);
	inline const Vector3 operator-(const Vector3& a, const Vector3& b);
	inline const Vector3 operator*(const Vector3& v, float s);
	inline const Vector3 operator*(float s, const Vector3& v);
	inline const Vector3 operator/(const Vector3& v, float s);
	inline Vector3& operator+=(Vector3& a, const Vector3& b);
	inline Vector3& operator-=(Vector3& a, const Vector3& b);
	inline Vector3& operator*=(Vector3& v, float s);
	inline Vector3& operator/=(Vector3& v, float s);
	inline const Vector3 Vector3Cross(const Vector3& a, const Vector3& b);
	inline float Vector3Dot(const Vector3& a, const Vector3& b);
	inline float Vector3Length(const Vector3& v);
	inline float Vector3LengthSquared(const Vector3& v);
	inline const Vector3 Vector3Lerp(const Vector3& a, const Vector3& b, float t);
	inline const Vector3 Vector3Normalize(const Vector3& v);

	/* Vector4 related functions */
	inline const Vector4 operator+(const Vector4& v);
	inline const Vector4 operator-(const Vector4& v);
	inline const Vector4 operator+(const Vector4& a, const Vector4& b);
	inline const Vector4 operator-(const Vector4& a, const Vector4& b);
	inline const Vector4 operator*(const Vector4& v, float s);
	inline const Vector4 operator*(float s, const Vector4& v);
	inline const Vector4 operator/(const Vector4& v, float s);
	inline const Vector4 operator*(const Vector4& v, const Matrix4x4& m);
	inline Vector4& operator*=(Vector4& v, const Matrix4x4& m);
	inline const Vector4 Vector4Lerp(const Vector4& a, const Vector4& b, float t);
	inline const Vector4 Vector4Saturate(const Vector4& v);

	/* Quaternion related functions */

	/* Matrix4x4 related functions */
	inline const Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b);
	inline Matrix4x4& operator*=(Matrix4x4& a, const Matrix4x4& b);
	inline const Matrix4x4 MatrixLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
	inline const Matrix4x4 MatrixPerspective(float fovy, float aspect, float near, float far);
	inline const Matrix4x4 MatrixRotateX(float angle);
	inline const Matrix4x4 MatrixRotateY(float angle);
	inline const Matrix4x4 MatrixRotateZ(float angle);
	inline const Matrix4x4 MatrixScale(float x, float y, float z);
	inline const Matrix4x4 MatrixTranslate(float x, float y, float z);
	inline const Matrix4x4 MatrixTranspose(const Matrix4x4& m);
}

/* General functions */

int32_t math::CeilToInt(float x)
{
	return static_cast<int32_t>(ceil(x));
}

int32_t math::FloorToInt(float x)
{
	return static_cast<int32_t>(floor(x));
}

int32_t math::Clamp(int32_t x, int32_t min, int32_t max)
{
	return x < min ? min : x > max ? max : x;
}

float math::Clamp(float x, float min, float max)
{
	return x < min ? min : x > max ? max : x;
}

int32_t math::Min(int32_t a, int32_t b)
{
	return a < b ? a : b;
}

float math::Min(float a, float b)
{
	return a < b ? a : b;
}

int32_t math::Max(int32_t a, int32_t b)
{
	return a > b ? a : b;
}

float math::Max(float a, float b)
{
	return a > b ? a : b;
}

float math::DegreesToRadians(float degrees)
{
	return degrees * (PI / 180.0f);
}

float math::RadiansToDegrees(float radians)
{
	return radians * (180.0f / PI);
}

/* float related functions */

float math::FloatLerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

float math::FloatSaturate(float f)
{
	return f < 0.0f ? 0.0f : f > 1.0f ? 1.0f : f;
}

float math::FloatFromUChar(uint8_t value)
{
	constexpr float inv_uchar_max = 1.0f / 255.0f;
	return static_cast<float>(value) * inv_uchar_max;
}

uint8_t math::FloatToUChar(float value)
{
	value = Clamp(value, 0.0f, 1.0f);
	return static_cast<uint8_t>(value * 255.0f);
}

const math::Vector2 math::operator+(const Vector2& v)
{
	return Vector2(v.x, v.y);
}

const math::Vector2 math::operator-(const Vector2& v)
{
	return Vector2(-v.x, -v.y);
}

const math::Vector2 math::operator+(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x + b.x, a.y + b.y);
}

const math::Vector2 math::operator-(const Vector2& a, const Vector2& b)
{
	return Vector2(a.x - b.x, a.y - b.y);
}

const math::Vector2 math::operator*(const Vector2& v, float s)
{
	return Vector2(v.x * s, v.y * s);
}

const math::Vector2 math::operator*(float s, const Vector2& v)
{
	return Vector2(v.x * s, v.y * s);
}

const math::Vector2 math::operator/(const Vector2& v, float s)
{
	const float inv_s = 1.0f / s;
	return Vector2(v.x * inv_s, v.y * inv_s);
}

math::Vector2& math::operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

math::Vector2& math::operator-=(Vector2& a, const Vector2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

math::Vector2& math::operator*=(Vector2& v, float s)
{
	v.x *= s;
	v.y *= s;
	return v;
}

math::Vector2& math::operator/=(Vector2& v, float s)
{
	const float inv_s = 1.0f / s;
	v.x *= inv_s;
	v.y *= inv_s;
	return v;
}

float math::Vector2Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

const math::Vector2 math::Vector2Lerp(const Vector2& a, const Vector2& b, float t)
{
	return a + (b - a) * t;
}

const math::Vector2 math::Vector2Min(const Vector2& a, const Vector2& b)
{
	return Vector2(Min(a.x, b.x), Min(a.y, b.y));
}

const math::Vector2 math::Vector2Max(const Vector2& a, const Vector2& b)
{
	return Vector2(Max(a.x, b.x), Max(a.y, b.y));
}

/* Vector3 related functions */

const math::Vector3 math::operator+(const Vector3& v)
{
	return Vector3(v.x, v.y, v.z);
}

const math::Vector3 math::operator-(const Vector3& v)
{
	return Vector3(-v.x, -v.y, -v.z);
}

const math::Vector3 math::operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

const math::Vector3 math::operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

const math::Vector3 math::operator*(const Vector3& v, float s)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}

const math::Vector3 math::operator*(float s, const Vector3& v)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}

const math::Vector3 math::operator/(const Vector3& v, float s)
{
	const float inv_s = 1.0f / s;
	return Vector3(v.x * inv_s, v.y * inv_s, v.z * inv_s);
}

math::Vector3& math::operator+=(Vector3& a, const Vector3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

math::Vector3& math::operator-=(Vector3& a, const Vector3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

math::Vector3& math::operator*=(Vector3& v, float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

math::Vector3& math::operator/=(Vector3& v, float s)
{
	const float inv_s = 1.0f / s;
	v.x *= inv_s;
	v.y *= inv_s;
	v.z *= inv_s;
	return v;
}

const math::Vector3 math::Vector3Cross(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x * b.y - a.y * b.x, a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z);
}

float math::Vector3Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float math::Vector3Length(const Vector3& v)
{
	return sqrtf(Vector3LengthSquared(v));
}

float math::Vector3LengthSquared(const Vector3& v)
{
	return Vector3Dot(v, v);
}

const math::Vector3 math::Vector3Lerp(const Vector3& a, const Vector3& b, float t)
{
	return a + (b - a) * t;
}

const math::Vector3 math::Vector3Normalize(const Vector3& v)
{
	return v / Vector3Length(v);
}

/* Vector4 related functions */

const math::Vector4 math::operator+(const Vector4& v)
{
	return Vector4(v.x, v.y, v.z, v.w);
}

const math::Vector4 math::operator-(const Vector4& v)
{
	return Vector4(-v.x, -v.y, -v.z, -v.w);
}

const math::Vector4 math::operator+(const Vector4& a, const Vector4& b)
{
	return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

const math::Vector4 math::operator-(const Vector4& a, const Vector4& b)
{
	return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

const math::Vector4 math::operator*(const Vector4& v, float s)
{
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

const math::Vector4 math::operator*(float s, const Vector4& v)
{
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

const math::Vector4 math::operator/(const Vector4& v, float s)
{
	const float inv_s = 1.0f / s;
	return Vector4(v.x * inv_s, v.y * inv_s, v.z * inv_s, v.w * inv_s);
}

const math::Vector4 math::operator*(const Vector4& v, const Matrix4x4& m)
{
	return Vector4(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
		v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]);
}

math::Vector4& math::operator*=(Vector4& v, const Matrix4x4& m)
{
	v = v * m;
	return v;
}

const math::Vector4 math::Vector4Lerp(const Vector4& a, const Vector4& b, float t)
{
	return a + (b - a) * t;
}

const math::Vector4 math::Vector4Saturate(const Vector4& v)
{
	math::Vector4 result;
	result.x = FloatSaturate(v.x);
	result.y = FloatSaturate(v.y);
	result.z = FloatSaturate(v.z);
	result.w = FloatSaturate(v.w);
	return result;
}

/* Matrix4x4 related functions */

const math::Matrix4x4 math::operator*(const Matrix4x4& a, const Matrix4x4& b)
{
	Matrix4x4 result{};
	for (int32_t i = 0; i < 4; ++i)
	{
		for (int32_t j = 0; j < 4; ++j)
		{
			for (int32_t k = 0; k < 4; ++k)
			{
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}

math::Matrix4x4& math::operator*=(Matrix4x4& a, const Matrix4x4& b)
{
	a = a * b;
	return a;
}

const math::Matrix4x4 math::MatrixLookAt(const Vector3& from, const Vector3& to, const Vector3& up)
{
	const Vector3 z_axis = Vector3Normalize(to - from);
	const Vector3 x_axis = Vector3Normalize(Vector3Cross(up, z_axis));
	const Vector3 y_axis = Vector3Cross(z_axis, x_axis);

	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[0][0] = x_axis.x;
	result.m[0][1] = x_axis.y;
	result.m[0][2] = x_axis.z;

	result.m[1][0] = y_axis.x;
	result.m[1][1] = y_axis.y;
	result.m[1][2] = y_axis.z;

	result.m[2][0] = z_axis.x;
	result.m[2][1] = z_axis.y;
	result.m[2][2] = z_axis.z;

	result.m[0][3] = -Vector3Dot(x_axis, from);
	result.m[1][3] = -Vector3Dot(y_axis, from);
	result.m[2][3] = -Vector3Dot(z_axis, from);

	return result;
}

const math::Matrix4x4 math::MatrixPerspective(float fovy, float aspect, float near, float far)
{
	const float z_range = far - near;
	SR_ASSERT(fovy > 0.0f && aspect > 0.0f);
	SR_ASSERT(near > 0.0f && far > 0.0f && z_range > 0.0f);

	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[1][1] = 1.0f / tanf(fovy / 2.0f);
	result.m[0][0] = result.m[1][1] / aspect;
	result.m[2][2] = -(near + far) / z_range;
	result.m[2][3] = -2.0f * near * far / z_range;
	result.m[3][2] = -1.0f;
	result.m[3][3] = 0.0f;

	return result;
}

const math::Matrix4x4 math::MatrixRotateX(float angle)
{
	const float cos = cosf(angle);
	const float sin = sinf(angle);
	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[1][1] = cos;
	result.m[1][2] = -sin;
	result.m[2][1] = sin;
	result.m[2][2] = cos;

	return result;
}

const math::Matrix4x4 math::MatrixRotateY(float angle)
{
	const float cos = cosf(angle);
	const float sin = sinf(angle);
	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[0][0] = cos;
	result.m[0][2] = sin;
	result.m[2][0] = -sin;
	result.m[2][2] = cos;

	return result;
}

const math::Matrix4x4 math::MatrixRotateZ(float angle)
{
	const float cos = cosf(angle);
	const float sin = sinf(angle);
	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[0][0] = cos;
	result.m[0][1] = -sin;
	result.m[1][0] = sin;
	result.m[1][1] = cos;

	return result;
}

const math::Matrix4x4 math::MatrixScale(float x, float y, float z)
{
	SR_ASSERT(x != 0.0f && y != 0.0f && z != 0.0f);

	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[0][0] = x;
	result.m[1][1] = y;
	result.m[2][2] = z;

	return result;
}

const math::Matrix4x4 math::MatrixTranslate(float x, float y, float z)
{
	Matrix4x4 result = MATRIX_IDENTITY;
	result.m[0][3] = x;
	result.m[1][3] = y;
	result.m[2][3] = z;

	return result;
}

const math::Matrix4x4 math::MatrixTranspose(const Matrix4x4& m)
{
	Matrix4x4 result;
	for (int32_t i = 0; i < 4; ++i)
	{
		for (int32_t j = 0; j < 4; ++j)
		{
			result.m[i][j] = m.m[j][i];
		}
	}

	return result;
}
