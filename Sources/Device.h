#pragma once

#include <windows.h>

#include "Resource.h"

namespace sr
{
	class Device
	{
	public:
		Device(HWND hWnd, uint32_t width, uint32_t height);
		~Device();

		bool Init();

		void Clear(uint32_t color);
		void Present();

		void SetPixel(int32_t x, int32_t y, uint32_t color);
		void SetDepth(int32_t x, int32_t y, float depth);

	private:
		HWND output_window_;
		HDC screen_dc_;
		HDC mem_dc_;
		HBITMAP mem_bitmap_;
		HBITMAP old_bitmap_;

		uint32_t width_;
		uint32_t height_;

		void* pixel_buffer_;
		Texture2D* zbuffer_;

		Buffer* vertex_buffer_;
		Buffer* index_buffer_;
	};
}
