#pragma once

class IShader;

struct FrameBuffer
{
	int32_t width;
	int32_t height;
	uint8_t* pixel_buffer;
	float* depth_buffer;
};

struct PipelineContext
{
	IShader* shader;
	int32_t sizeof_attributes;
	int32_t sizeof_varyings;
	int32_t sizeof_constants;
	bool two_sided;
	bool enable_blend;
	void* shader_attributes[3];
	void* shader_varyings;
	void* shader_constants;
};

struct Point
{
	int32_t x;
	int32_t y;
};

struct Rect
{
	int32_t min_x;
	int32_t min_y;
	int32_t max_x;
	int32_t max_y;
};
