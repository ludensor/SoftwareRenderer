#pragma once

#include <windows.h>
#include <memory>

#include "Buffer.h"
#include "Shader.h"
#include "Texture2D.h"

namespace sr
{
	struct Vertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMVECTOR color;
	};

	struct Edge
	{
		Vertex v1, v2;
	};

	struct Triangle
	{
		float top_y, bottom_y;
		Edge left, right;
	};

	class Device
	{
	public:
		Device(HWND hWnd, uint32_t width, uint32_t height);

		bool StartUp();
		void ShutDown();

		std::shared_ptr<Buffer> CreateBuffer(const void* system_memory, uint32_t byte_width);
		std::shared_ptr<Texture2D> CreateTexture2D(const void* system_memory, uint32_t width, uint32_t height);


		void SetVertexBuffer(std::shared_ptr<Buffer> vertex_buffer, uint32_t stride);
		void SetIndexBuffer(std::shared_ptr<Buffer> index_buffer);
		void Clear(const DirectX::XMFLOAT4& color);
		void Present();
		void DrawPrimitive(uint32_t vertex_count);
		void DrawIndexedPrimitive(uint32_t index_count);
		void Draw(Vertex p1, Vertex p2, Vertex p3);

	private:
		int32_t BuildTriangles(Triangle triangles[2], const Vertex* top, const Vertex* middle, const Vertex* bottom);
		void DrawTriangle(const Triangle& triangle);
		void DrawScanline(float xl, float xr, uint32_t y, DirectX::XMVECTOR cl, DirectX::XMVECTOR cr);

		DirectX::XMFLOAT4 TransformNDC(const DirectX::XMFLOAT4& v);

		uint32_t ToColorRef(const DirectX::XMFLOAT4& color);

	private:
		HWND output_window_;
		HDC screen_dc_;
		HDC mem_dc_;
		HBITMAP mem_bitmap_;
		HBITMAP old_bitmap_;

		uint32_t width_;
		uint32_t height_;

		void* pixel_buffer_;
		std::shared_ptr<Texture2D> zbuffer_;

		std::shared_ptr<Buffer> vertex_buffer_;
		std::shared_ptr<Buffer> index_buffer_;
		uint32_t stride_;

		std::shared_ptr<IShader> shader_;
	};
}
