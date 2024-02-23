#pragma once

#include <intrin.h>

#include "Archive.h"
#include <DirectXMath.h>

namespace sr
{
	namespace math
	{
		typedef __m128 VectorRegister;

		struct alignas(16) Vectorf32
		{
			union
			{
				float f[4];
				VectorRegister v;
			};

			inline operator VectorRegister() const noexcept { return v; }
			inline operator const float* () const noexcept { return f; }
		};

		struct alignas(16) MatrixRegister
		{
			VectorRegister r[4];

			constexpr MatrixRegister(VectorRegister r0, VectorRegister r1, VectorRegister r2, VectorRegister r3)
				: r{ r0, r1, r2, r3 } {}
		};

		struct Vector3
		{
			float x, y, z;
		};

		struct Vector4
		{
			float x, y, z, w;

			const Vector4 operator+() const;
			const Vector4 operator-() const;
			const Vector4 operator+(const Vector4& v) const;
			const Vector4 operator-(const Vector4& v) const;
			const Vector4 operator*(float s) const;
			const Vector4 operator/(float s) const;
			Vector4& operator+=(const Vector4& v);
			Vector4& operator-=(const Vector4& v);
			Vector4& operator*=(float s);
			Vector4& operator/=(float s);
		};

		constexpr Vectorf32 ZERO{ 0.0f, 0.0f, 0.0f ,0.0f };
		constexpr Vectorf32 ONE{ 1.0f, 1.0f, 1.0f, 1.0f };
	}
}

inline sr::Archive& operator>>(sr::Archive& ar, sr::math::VectorRegister& v)
{
	ar.Read(&v, sizeof(v));
	return ar;
}

inline sr::Archive& operator<<(sr::Archive& ar, sr::math::VectorRegister v)
{
	ar.Write(&v, sizeof(v));
	return ar;
}

inline sr::Archive& operator>>(sr::Archive& ar, sr::math::MatrixRegister& m)
{
	ar >> m.r[0] >> m.r[1] >> m.r[2] >> m.r[3];
	return ar;
}

inline sr::Archive& operator<<(sr::Archive& ar, const sr::math::MatrixRegister& m)
{
	ar << m.r[0] << m.r[1] << m.r[2] << m.r[3];
	return ar;
}

inline const sr::math::Vector4 sr::math::Vector4::operator+() const
{
	return Vector4{ +x, +y, +z, +w };
}

inline const sr::math::Vector4 sr::math::Vector4::operator-() const
{
	return Vector4{ -x, -y, -z, -w };
}

inline const sr::math::Vector4 sr::math::Vector4::operator+(const Vector4& v) const
{
	return Vector4{ x + v.x , y + v.y, z + v.z, w + v.w };
}

inline const sr::math::Vector4 sr::math::Vector4::operator-(const Vector4& v) const
{
	return Vector4{ x - v.x, y - v.y, z - v.z, w - v.w };
}

inline const sr::math::Vector4 sr::math::Vector4::operator*(float s) const
{
	return Vector4{ x * s, y * s, z * s, w * s };
}

inline const sr::math::Vector4 sr::math::Vector4::operator/(float s) const
{
	const float inv_s = 1.0f / s;
	return Vector4{ x * inv_s, y * inv_s, z * inv_s, w * inv_s };
}	

inline sr::math::Vector4& sr::math::Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline sr::math::Vector4& sr::math::Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline sr::math::Vector4& sr::math::Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

inline sr::math::Vector4& sr::math::Vector4::operator/=(float s)
{
	const float inv_s = 1.0f / s;
	x *= inv_s;
	y *= inv_s;
	z *= inv_s;
	w *= inv_s;
	return *this;
}
