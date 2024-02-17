#pragma once

#include "MathUtils.h"

namespace sr
{
	struct VertexShaderData
	{
		virtual ~VertexShaderData() = default;
	};

	struct PixelShaderData
	{
		virtual ~PixelShaderData() = default;
	};

	class IShader
	{
		virtual void SetVertexShaderConstantBuffer(void* buffer, uint32_t buffer_size) = 0;
		virtual void SetPixelShaderConstantBuffer(void* buffer, uint32_t buffer_size) = 0;

		virtual PixelShaderData VertexShader(VertexShaderData input) = 0;
		virtual Vector4 PixelShader(PixelShaderData input) = 0;
	};
}
