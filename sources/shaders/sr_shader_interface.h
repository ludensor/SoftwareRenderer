#pragma once

#include "core/sr_math.h"

enum class SHADER_MODE : uint8_t
{
	FLAT,
	GOURAUD,
	PHONG,
	BLINN_PHONG,
};

class IShader
{
public:
	virtual math::Vector4 VertexShader(void* varyings, const void* attributes, const void* constants) = 0;
	virtual math::Vector4 PixelShader(const void* varyings, const void* constants, bool& discard) = 0;
};
