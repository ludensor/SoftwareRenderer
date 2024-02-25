#include "Device.h"

#define Clamp(x, min, max) (x < min ? min : x > max ? max : x)

sr::Device::Device(HWND hWnd, uint32_t width, uint32_t height)
	: output_window_(hWnd)
	, screen_dc_(nullptr)
	, mem_dc_(nullptr)
	, mem_bitmap_(nullptr)
	, old_bitmap_(nullptr)
	, width_(width)
	, height_(height)
	, pixel_buffer_(nullptr)
	, zbuffer_(nullptr)
	, vertex_buffer_(nullptr)
	, index_buffer_(nullptr)
	, stride_(0)
{

}

bool sr::Device::StartUp()
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

void sr::Device::ShutDown()
{
	SelectObject(mem_dc_, old_bitmap_);
	DeleteObject(mem_bitmap_);
	DeleteDC(mem_dc_);
	ReleaseDC(output_window_, screen_dc_);
}

std::shared_ptr<sr::Buffer> sr::Device::CreateBuffer(const void* system_memory, uint32_t byte_width)
{
	return std::make_shared<Buffer>(system_memory, byte_width);
}

std::shared_ptr<sr::Texture2D> sr::Device::CreateTexture2D(const void* system_memory, uint32_t width, uint32_t height)
{
	return std::make_shared<Texture2D>(system_memory, width, height);
}

void sr::Device::SetVertexBuffer(std::shared_ptr<Buffer> vertex_buffer, uint32_t stride)
{
	vertex_buffer_ = vertex_buffer;
	stride_ = stride;
}

void sr::Device::SetIndexBuffer(std::shared_ptr<Buffer> index_buffer)
{
	index_buffer_ = index_buffer;
}

void sr::Device::Clear(const DirectX::XMFLOAT4& color)
{
	uint32_t* data = (uint32_t*)pixel_buffer_;

	const uint32_t argb = ToColorRef(color);
	for (uint32_t i = 0; i < width_; ++i)
	{
		data[i] = argb;
	}

	for (uint32_t i = 1; i < height_; ++i)
	{
		const uint32_t byte_width = width_ * 4;
		memcpy(&data[i * width_], data, byte_width);
	}
}

void sr::Device::Present()
{
	BitBlt(screen_dc_, 0, 0, width_, height_, mem_dc_, 0, 0, SRCCOPY);
}

void sr::Device::DrawPrimitive(uint32_t vertex_count)
{
	if (vertex_buffer_)
	{
		void* buffer = vertex_buffer_->GetData();
	}
}

void sr::Device::DrawIndexedPrimitive(uint32_t index_count)
{

}

void sr::Device::Draw(Vertex p1, Vertex p2, Vertex p3)
{
	Triangle triangles[2];
	p1.position = TransformNDC(p1.position);
	p2.position = TransformNDC(p2.position);
	p3.position = TransformNDC(p3.position);

	int32_t num_triangles = BuildTriangles(triangles, &p1, &p2, &p3);

	if (num_triangles >= 1) { DrawTriangle(triangles[0]); }
	if (num_triangles >= 2) { DrawTriangle(triangles[1]); }
}

int32_t sr::Device::BuildTriangles(Triangle triangles[2], const Vertex* top, const Vertex* middle, const Vertex* bottom)
{
	if (top->position.x == middle->position.x && middle->position.x == bottom->position.x) { return 0; }
	if (top->position.y == middle->position.y && middle->position.y == bottom->position.y) { return 0; }
	if (top->position.y > middle->position.y) { std::swap(top, middle); }
	if (top->position.y > bottom->position.y) { std::swap(top, bottom); }
	if (middle->position.y > bottom->position.y) { std::swap(middle, bottom); }

	//    T
	//  /   \
	// M  -  B
	if (middle->position.y == bottom->position.y)
	{
		if (middle->position.x > bottom->position.x) { std::swap(middle, bottom); }
		triangles[0].top_y = top->position.y;
		triangles[0].bottom_y = bottom->position.y;
		triangles[0].left.v1 = *top;
		triangles[0].left.v2 = *middle;
		triangles[0].right.v1 = *top;
		triangles[0].right.v2 = *bottom;
		return 1;
	}

	// T  -  M
	//  \   /
	//    B
	if (top->position.y == middle->position.y)
	{
		if (top->position.x > middle->position.x) { std::swap(top, middle); }
		triangles[0].top_y = top->position.y;
		triangles[0].bottom_y = bottom->position.y;
		triangles[0].left.v1 = *top;
		triangles[0].left.v2 = *bottom;
		triangles[0].right.v1 = *middle;
		triangles[0].right.v2 = *bottom;
		return 1;
	}

	//   / T    T \
	// M   | or |   M
	//   \ B    B /

	// top - bottom 직선과 middle을 지나는 수평선이 만나는 지점의 x 좌표 구하기
	// b = y - ax
	const float a = (bottom->position.y - top->position.y) / (bottom->position.x - top->position.x);
	const float b = top->position.y - top->position.x * a;
	// x = (y - b) / a
	const float x = (middle->position.y - b) / a;
	const float t = (middle->position.y - top->position.y) / (bottom->position.y - top->position.y);

	triangles[0].top_y = top->position.y;
	triangles[0].bottom_y = middle->position.y;
	triangles[1].top_y = middle->position.y;
	triangles[1].bottom_y = bottom->position.y;

	if (middle->position.x > x)
	{
		triangles[0].left.v1 = *top;
		triangles[0].left.v2.position.x = top->position.x + (bottom->position.x - top->position.x) * t;
		triangles[0].left.v2.position.y = top->position.y + (bottom->position.y - top->position.y) * t;
		triangles[0].left.v2.color = DirectX::XMVectorLerp(top->color, bottom->color, t);
		triangles[0].right.v1 = *top;
		triangles[0].right.v2 = *middle;
		triangles[1].left.v1 = triangles[0].left.v2;
		triangles[1].left.v2 = *bottom;
		triangles[1].right.v1 = *middle;
		triangles[1].right.v2 = *bottom;
	}
	else
	{
		triangles[0].left.v1 = *top;
		triangles[0].left.v2 = *middle;
		triangles[0].right.v1 = *top;
		triangles[0].right.v2.position.x = top->position.x + (bottom->position.x - top->position.x) * t;
		triangles[0].right.v2.position.y = top->position.y + (bottom->position.y - top->position.y) * t;
		triangles[0].right.v2.color = DirectX::XMVectorLerp(top->color, bottom->color, t);
		triangles[1].left.v1 = *middle;
		triangles[1].left.v2 = *bottom;
		triangles[1].right.v1 = triangles[0].right.v2;
		triangles[1].right.v2 = *bottom;
	}

	return 2;
}

void sr::Device::DrawTriangle(const Triangle& triangle)
{
	// left
	const float diff_xl = triangle.left.v2.position.x - triangle.left.v1.position.x;
	const float diff_yl = triangle.left.v2.position.y - triangle.left.v1.position.y;

	// right
	const float diff_xr = triangle.right.v2.position.x - triangle.right.v1.position.x;
	const float diff_yr = triangle.right.v2.position.y - triangle.right.v1.position.y;

	float factor_xl = 0.0f;
	float factor_xr = 0.0f;
	const float factor_step_xl = 1.0f / diff_yl;
	const float factor_step_xr = 1.0f / diff_yr;

	const uint32_t begin_y = Clamp((uint32_t)triangle.top_y, 0, height_);
	const uint32_t end_y = Clamp((uint32_t)triangle.bottom_y, 0, height_);
	for (uint32_t y = begin_y; y < end_y; ++y)
	{
		const float xl = triangle.left.v1.position.x + diff_xl * factor_xl;
		const float xr = triangle.right.v1.position.x + diff_xr * factor_xr;

		const DirectX::XMVECTOR color_l = DirectX::XMVectorLerp(triangle.left.v1.color, triangle.left.v2.color, factor_xl);
		const DirectX::XMVECTOR color_r = DirectX::XMVectorLerp(triangle.right.v1.color, triangle.right.v2.color, factor_xr);
		DrawScanline(xl, xr, y, color_l, color_r);
		factor_xl += factor_step_xl;
		factor_xr += factor_step_xr;
	}
}

void sr::Device::DrawScanline(float xl, float xr, uint32_t y, DirectX::XMVECTOR cl, DirectX::XMVECTOR cr)
{
	const float diff_x = xr - xl;
	if (diff_x <= 0.0f)
	{
		return;
	}

	uint32_t* data = (uint32_t*)pixel_buffer_;

	float factor = 0.0f;
	const float factor_step = 1.0f / diff_x;
	const uint32_t begin_x = Clamp((uint32_t)xl, 0, width_);
	const uint32_t end_x = Clamp((uint32_t)xr, 0, width_);
	for (uint32_t x = begin_x; x < end_x; ++x)
	{
		DirectX::XMVECTOR color_register = DirectX::XMVectorLerp(cl, cr, factor);
		DirectX::XMFLOAT4 color;
		DirectX::XMStoreFloat4(&color, color_register);
		factor += factor_step;
		/*Archive ar;
		ar << color_register;
		data[y * width_ + x] = math::ColorRGBATo32ARGB(pixel_shader_->PS(ar));*/
		data[y * width_ + x] = ToColorRef(color);
	}
}

DirectX::XMFLOAT4 sr::Device::TransformNDC(const DirectX::XMFLOAT4& v)
{
	DirectX::XMFLOAT4 result;
	const float rhw = 1.0f / v.w;
	result.x = (v.x * rhw + 1.0f) * width_ * 0.5f;
	result.y = (1.0f - v.y * rhw) * height_ * 0.5f;
	result.z = v.z * rhw;
	result.w = 1.0f;
	return result;
}

uint32_t sr::Device::ToColorRef(const DirectX::XMFLOAT4& color)
{
	const uint8_t r = (uint8_t)(Clamp(color.x, 0.0f, 1.0f) * 255.0f);
	const uint8_t g = (uint8_t)(Clamp(color.y, 0.0f, 1.0f) * 255.0f);
	const uint8_t b = (uint8_t)(Clamp(color.z, 0.0f, 1.0f) * 255.0f);
	const uint8_t a = (uint8_t)(Clamp(color.w, 0.0f, 1.0f) * 255.0f);

	return (b) | (g << 8) | (r << 16) | (a << 24);
}

