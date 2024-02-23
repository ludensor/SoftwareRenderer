#pragma once

#include "MathTypes.h"

namespace sr
{
	namespace math
	{
		VectorRegister LoadFloat4(const Vector4* src) noexcept;
		void StoreFloat4(Vector4* dst, VectorRegister v) noexcept;

		VectorRegister VectorSet(float x, float y, float z, float w) noexcept;
		VectorRegister VectorScale(VectorRegister v, float scale) noexcept;
		VectorRegister VectorClamp(VectorRegister v, VectorRegister min, VectorRegister max) noexcept;
		VectorRegister VectorInterp(VectorRegister v1, VectorRegister v2, float t) noexcept;

		uint32_t ColorRGBATo32ARGB(const Vector4* rgba) noexcept;
		uint32_t ColorRGBATo32ARGB(VectorRegister rgba) noexcept;

		uint32_t ToColorRef(const Vector4& color);
	}
}

inline sr::math::VectorRegister sr::math::LoadFloat4(const Vector4* src) noexcept
{
	return _mm_loadu_ps(&src->x);
}

inline void sr::math::StoreFloat4(Vector4* dst, VectorRegister v) noexcept
{
	_mm_storeu_ps(&dst->x, v);
}

inline sr::math::VectorRegister sr::math::VectorSet(float x, float y, float z, float w) noexcept
{
	return _mm_set_ps(w, z, y, x);
}

inline sr::math::VectorRegister sr::math::VectorScale(VectorRegister v, float scale) noexcept
{
	VectorRegister result = _mm_set_ps1(scale);
	return _mm_mul_ps(v, result);
}

inline sr::math::VectorRegister sr::math::VectorClamp(VectorRegister v, VectorRegister min, VectorRegister max) noexcept
{
	VectorRegister result;
	result = _mm_max_ps(v, min);
	result = _mm_min_ps(result, max);
	return result;
}

inline sr::math::VectorRegister sr::math::VectorInterp(VectorRegister v1, VectorRegister v2, float t) noexcept
{
	VectorRegister l = _mm_sub_ps(v2, v1);
	VectorRegister s = _mm_set_ps1(t);
	return _mm_add_ps(_mm_mul_ps(l, s), v1);
}

inline uint32_t sr::math::ColorRGBATo32ARGB(const Vector4* rgba) noexcept
{
	return ColorRGBATo32ARGB(LoadFloat4(rgba));
}

inline uint32_t sr::math::ColorRGBATo32ARGB(VectorRegister rgba) noexcept
{
	VectorRegister color = VectorScale(VectorClamp(rgba, ZERO, ONE), 255.0f);
	Vector4 result;
	StoreFloat4(&result, color);

	const uint8_t r = (uint8_t)result.x;
	const uint8_t g = (uint8_t)result.y;
	const uint8_t b = (uint8_t)result.z;
	const uint8_t a = (uint8_t)result.w;

	return (b) | (g << 8) | (r << 16) | (a << 24);
}

inline uint32_t sr::math::ToColorRef(const Vector4& color)
{
	const uint8_t r = (uint8_t)((color.x < 0.0f ? 0.0f : color.x > 1.0f ? 1.0f : color.x) * 255.0f);
	const uint8_t g = (uint8_t)((color.y < 0.0f ? 0.0f : color.y > 1.0f ? 1.0f : color.y) * 255.0f);
	const uint8_t b = (uint8_t)((color.z < 0.0f ? 0.0f : color.z > 1.0f ? 1.0f : color.z) * 255.0f);
	const uint8_t a = (uint8_t)((color.w < 0.0f ? 0.0f : color.w > 1.0f ? 1.0f : color.w) * 255.0f);
	return (b) | (g << 8) | (r << 16) | (a << 24);
}
