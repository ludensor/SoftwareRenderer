#pragma once

#include "core/sr_core_types.h"
#include "core/sr_math.h"

struct Edge
{
	math::Vector2* screen_coord1;
	math::Vector2* screen_coord2;
	void* varyings1;
	void* varyings2;
};

struct Trapezoid
{
	float top;
	float bottom;
	Edge left;
	Edge right;
};

class Rasterizer
{
public:
	Rasterizer() = default;
	~Rasterizer() = default;

	void RasterizeTriangle_V1(const FrameBuffer& frame_buffer, PipelineContext& context, const math::Vector4 clip_coords[3], void* varyings[3]);

private:
	bool IsBackFacing(const math::Vector3 ndc_coords[3]);
	math::Vector3 ViewportTransform(int32_t width, int32_t height, const math::Vector3& ndc_coords);
	int32_t SeparateTriangle_V1(Trapezoid trapezoid[2], math::Vector2 screen_coords[3], void* varyings[3]);
	void InterpolateVaryings_V1(const Trapezoid& trapezoid, void* dst_varyings, int32_t sizeof_varyings, float tx, float ty1, float ty2);
	void DrawFragment(const FrameBuffer& frame_buffer, PipelineContext& context, int32_t index);
};
