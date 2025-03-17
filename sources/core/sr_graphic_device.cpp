#include "sr_pch.h"
#include "core/sr_graphic_device.h"
#include "core/sr_rasterizer.h"
#include "shaders/sr_flat_shader.h"

GraphicDevice::GraphicDevice()
	: width_(800)
	, height_(600)
	, shader_(nullptr)
{
	const int32_t buffer_bytes = width_ * height_ * 4;
	pixel_buffer_ = reinterpret_cast<uint8_t*>(malloc(buffer_bytes));
	depth_buffer_ = reinterpret_cast<float*>(malloc(buffer_bytes));

	rasterizer_ = new Rasterizer();

	pipeline_context_ = CreatePipelineContext(sizeof(FlatAttributeData), sizeof(FlatVertexData), sizeof(FlatConstantData), false, false);
}

GraphicDevice::~GraphicDevice()
{
	free(pixel_buffer_);
	free(depth_buffer_);

	delete rasterizer_;

	ReleasePipelineContext(pipeline_context_);
}

void GraphicDevice::Initialize()
{
	SelectShader(SHADER_MODE::FLAT);
}

void GraphicDevice::Finalize()
{
	DeleteShader();
}

int32_t GraphicDevice::GetWidth() const
{
	return width_;
}

int32_t GraphicDevice::GetHeight() const
{
	return height_;
}

uint8_t* GraphicDevice::GetPixelBuffer() const
{
	return pixel_buffer_;
}

float* GraphicDevice::GetDepthBuffer() const
{
	return depth_buffer_;
}

void GraphicDevice::ClearPixelBuffer(const math::Vector4& clear_color)
{
	const uint8_t r = math::FloatToUChar(clear_color.x);
	const uint8_t g = math::FloatToUChar(clear_color.y);
	const uint8_t b = math::FloatToUChar(clear_color.z);
	const uint8_t a = math::FloatToUChar(clear_color.w);
	const uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;
	const int32_t num_pixels = width_ * height_;
	CopyBuffer(reinterpret_cast<uint32_t*>(pixel_buffer_), color, num_pixels);
}

void GraphicDevice::ClearDepthBuffer(float clear_depth)
{
	const int32_t num_pixels = width_ * height_;
	CopyBuffer(depth_buffer_, clear_depth, num_pixels);
}

PipelineContext* GraphicDevice::CreatePipelineContext(int32_t sizeof_attributes, int32_t sizeof_varyings, int32_t sizeof_constants, bool two_sided, bool enable_blend)
{
	SR_ASSERT(sizeof_varyings > 0);
	SR_ASSERT(sizeof_varyings % sizeof(float) == 0);

	PipelineContext* context = reinterpret_cast<PipelineContext*>(malloc(sizeof(PipelineContext)));
	SR_ASSERT(context);
	memset(context, 0, sizeof(PipelineContext));

	context->sizeof_attributes = sizeof_attributes;
	context->sizeof_varyings = sizeof_varyings;
	context->sizeof_constants = sizeof_constants;
	context->two_sided = two_sided;
	context->enable_blend = enable_blend;

	for (int32_t i = 0; i < 3; ++i)
	{
		context->shader_attributes[i] = malloc(sizeof_attributes);
		SR_ASSERT(context->shader_attributes[i]);
		memset(context->shader_attributes[i], 0, sizeof_attributes);
	}

	context->shader_varyings = malloc(sizeof_varyings);
	SR_ASSERT(context->shader_varyings);
	memset(context->shader_varyings, 0, sizeof_varyings);

	context->shader_constants = malloc(sizeof_constants);
	SR_ASSERT(context->shader_constants);
	memset(context->shader_constants, 0, sizeof_constants);

	return context;
}

void GraphicDevice::ReleasePipelineContext(PipelineContext* context)
{
	SR_ASSERT(context);

	for (int32_t i = 0; i < 3; ++i)
	{
		free(context->shader_attributes[i]);
	}

	free(context->shader_varyings);
	free(context->shader_constants);

	free(context);
}

IShader* GraphicDevice::SelectShader(SHADER_MODE mode)
{
	DeleteShader();

	switch (mode)
	{
	case SHADER_MODE::FLAT:
		shader_ = new FlatShader();
		break;
	case SHADER_MODE::GOURAUD:
		break;
	case SHADER_MODE::PHONG:
		break;
	case SHADER_MODE::BLINN_PHONG:
		break;
	}

	return shader_;
}

void GraphicDevice::DeleteShader()
{
	if (shader_)
	{
		delete shader_;
		shader_ = nullptr;
	}
}

IShader* GraphicDevice::GetShader() const
{
	return shader_;
}

void GraphicDevice::Draw()
{
	FrameBuffer frame_buffer;
	frame_buffer.width = width_;
	frame_buffer.height = height_;
	frame_buffer.pixel_buffer = pixel_buffer_;
	frame_buffer.depth_buffer = depth_buffer_;

	FlatVertexData in_varyings[3]
	{
		{ math::Vector4(-0.0f, +0.5f, 1.0f, 1.0f), math::Vector4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ math::Vector4(-0.5f, -0.5f, 1.0f, 1.0f), math::Vector4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ math::Vector4(+0.5f, -0.5f, 1.0f, 1.0f), math::Vector4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	math::Vector4 vertices[3] = { in_varyings[0].position, in_varyings[1].position, in_varyings[2].position };

	void* varyings[3];
	varyings[0] = reinterpret_cast<void*>(&in_varyings[0]);
	varyings[1] = reinterpret_cast<void*>(&in_varyings[1]);
	varyings[2] = reinterpret_cast<void*>(&in_varyings[2]);

	pipeline_context_->shader = shader_;
	rasterizer_->RasterizeTriangle_V2(frame_buffer, *pipeline_context_, vertices, varyings);
}
