#pragma once

#include "Shader.h"

namespace sr
{
	struct ColorVertexShaderConstantBuffer
	{
		Matrix4x4 world_matrix;
		Matrix4x4 view_matrix;
		Matrix4x4 projection_matrix;
	};

	struct ColorVertexShaderData : public VertexShaderData
	{
		Vector4 position;
		Vector4 color;
	};

	struct ColorPixelShaderData : public PixelShaderData
	{
		Vector4 position;
		Vector4 color;
	};

	class ColorShader : public IShader
	{
		virtual void SetVertexShaderConstantBuffer(void* buffer, uint32_t buffer_size);

		virtual PixelShaderData VertexShader(VertexShaderData input);
		virtual Vector4 PixelShader(PixelShaderData input);

	private:
		ColorVertexShaderConstantBuffer buffer_;
	};
}
