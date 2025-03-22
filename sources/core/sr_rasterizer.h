#pragma once

#include "core/sr_core_types.h"
#include "core/sr_math.h"

namespace rasterizer
{
	/*
	 * Clipping
	 * Perspective Division
	 * Backface Culling
	 * Viewport Mapping
	 * Scan Transform
	 * Depth Test
	 * Pixel Shader
	 * Blending
	 * Output Merge
	 */

	 /* 1.rasterization scanline */
	void RasterizeTriangle_V1(const FrameBuffer& frame_buffer, PipelineContext& context, const math::Vector4 clip_coords[3], void* varyings[3]);

	/* 2.rasterization barycentric coordinate */
	void RasterizeTriangle_V2(const FrameBuffer& frame_buffer, PipelineContext& context, const math::Vector4 clip_coords[3], void* varyings[3]);
}
