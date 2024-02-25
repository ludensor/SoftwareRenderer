#pragma once

#include <DirectXMath.h>

namespace sr
{
	struct alignas(16) VertexData
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR color;
	};

	struct alignas(16) PixelData
	{
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR color;
	};

	class IShader
	{
	public:
		virtual PixelData VS(const VertexData& input) = 0;
		virtual DirectX::XMVECTOR PS(const PixelData& input) = 0;
	};

	class ShaderColor
	{
	public:
		virtual PixelData VS(const VertexData& input) final;
		virtual DirectX::XMVECTOR PS(const PixelData& input) final;

	private:
		DirectX::XMVECTOR world_matrix_;
		DirectX::XMVECTOR view_matrix_;
		DirectX::XMVECTOR projection_matrix_;
	};
}
