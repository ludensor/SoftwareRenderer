#include "Device.h"

sr::Device::Device(HWND hWnd, uint32_t width, uint32_t height)
	: output_window_(hWnd)
	, screen_dc_(nullptr)
	, mem_dc_(nullptr)
	, mem_bitmap_(nullptr)
	, old_bitmap_(nullptr)
	, width_(width)
	, height_(height)
	, pixel_buffer_(nullptr)
	, vertex_buffer_(nullptr)
	, index_buffer_(nullptr)
{
	zbuffer_ = new Texture2D(width_, height_);
}

sr::Device::~Device()
{
	if (vertex_buffer_) { delete vertex_buffer_; }
	if (index_buffer_) { delete index_buffer_; }
	if (zbuffer_) { delete zbuffer_; }

	SelectObject(mem_dc_, old_bitmap_);
	DeleteObject(mem_bitmap_);
	DeleteDC(mem_dc_);
	ReleaseDC(output_window_, screen_dc_);
}

bool sr::Device::Init()
{
	screen_dc_ = GetDC(output_window_);
	mem_dc_ = CreateCompatibleDC(screen_dc_);

	BITMAPINFO bi{};
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = (LONG)width_;
	bi.bmiHeader.biHeight = -(LONG)height_;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	mem_bitmap_ = CreateDIBSection(screen_dc_, &bi, DIB_RGB_COLORS, &pixel_buffer_, 0, 0);
	if (!mem_bitmap_)
	{
		return false;
	}

	old_bitmap_ = (HBITMAP)SelectObject(mem_dc_, mem_bitmap_);

	return true;
}

void sr::Device::Clear(uint32_t color)
{
	uint32_t* data = (uint32_t*)pixel_buffer_;

	for (uint32_t i = 0; i < width_; ++i)
	{
		data[i] = color;
	}

	for (uint32_t i = 1; i < height_; ++i)
	{
		const uint32_t byte_width = width_ * 4;
		memcpy(&data[i * width_], data, byte_width);
	}
}

void sr::Device::SetPixel(int32_t x, int32_t y, uint32_t color)
{
	if ((uint32_t)x < width_ && (uint32_t)y < height_)
	{
		uint32_t* data = (uint32_t*)pixel_buffer_;
		data[y * width_ + x] = color;
	}
}

void sr::Device::SetDepth(int32_t x, int32_t y, float depth)
{
	if ((uint32_t)x < width_ && (uint32_t)y < height_)
	{
		float* data = (float*)zbuffer_->GetData();
		data[y * width_ + x] = depth;
	}
}

void sr::Device::Present()
{
	BitBlt(screen_dc_, 0, 0, width_, height_, mem_dc_, 0, 0, SRCCOPY);
}

