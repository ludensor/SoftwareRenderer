#pragma once

#include "core/sr_core_types.h"
#include "core/sr_math.h"

class Rasterizer;
enum class SHADER_MODE : uint8_t;

class GraphicDevice
{
public:
	GraphicDevice();
	~GraphicDevice();

	void Initialize();
	void Finalize();

	int32_t GetWidth() const;
	int32_t GetHeight() const;

	uint8_t* GetPixelBuffer() const;
	float* GetDepthBuffer() const;

	void ClearPixelBuffer(const math::Vector4& clear_color);
	void ClearDepthBuffer(float clear_depth);

	PipelineContext* CreatePipelineContext(int32_t sizeof_attributes, int32_t sizeof_varyings, int32_t sizeof_constants, bool two_sided, bool enable_blend);
	void ReleasePipelineContext(PipelineContext* context);

	IShader* SelectShader(SHADER_MODE mode);
	void DeleteShader();
	IShader* GetShader() const;

	void Draw();

private:
	template<typename BufferType>
	void CopyBuffer(BufferType* dst, BufferType src, int32_t count) const;

private:
	uint8_t* pixel_buffer_;
	float* depth_buffer_;

	int32_t width_;
	int32_t height_;

	IShader* shader_;
	Rasterizer* rasterizer_;

	PipelineContext* pipeline_context_;
};

template<typename BufferType>
inline void GraphicDevice::CopyBuffer(BufferType* dst, BufferType src, int32_t count) const
{
	if (count == 0)
	{
		return;
	}

	if (count == 1)
	{
		memcpy(dst, &src, sizeof(BufferType));
	}
	else
	{
		const int32_t half = count >> 1;
		CopyBuffer(dst, src, half);
		memcpy(dst + half, dst, sizeof(BufferType) * half);

		if (count % 2)
		{
			memcpy(dst + (count - 1), &src, sizeof(BufferType));
		}
	}
}
