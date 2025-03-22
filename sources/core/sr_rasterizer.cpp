#include "sr_pch.h"
#include "core/sr_rasterizer.h"
#include "shaders/sr_shader_interface.h"

struct Edge
{
	math::Vector2 screen_coord1;
	math::Vector2 screen_coord2;
	float screen_depth1;
	float screen_depth2;
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

static int32_t MakeTrapezoid_V1(Trapezoid trapezoid[2], const math::Vector2 screen_coords[3], const float screen_depth[3], void* varyings[3])
{
	int32_t top_index = 0;
	int32_t middle_index = 1;
	int32_t bottom_index = 2;

	if (screen_coords[top_index].y > screen_coords[middle_index].y) { std::swap(top_index, middle_index); }
	if (screen_coords[top_index].y > screen_coords[bottom_index].y) { std::swap(top_index, bottom_index); }
	if (screen_coords[middle_index].y > screen_coords[bottom_index].y) { std::swap(middle_index, bottom_index); }
	if (screen_coords[top_index].x == screen_coords[middle_index].x && screen_coords[middle_index].x == screen_coords[bottom_index].x) { return 0; }
	if (screen_coords[top_index].y == screen_coords[middle_index].y && screen_coords[middle_index].y == screen_coords[bottom_index].y) { return 0; }

	//    T
	//  /   \
	// M  -  B
	if (screen_coords[top_index].y != screen_coords[middle_index].y && screen_coords[middle_index].y == screen_coords[bottom_index].y)
	{
		if (screen_coords[middle_index].x > screen_coords[bottom_index].x) { std::swap(middle_index, bottom_index); }

		trapezoid[0].top = screen_coords[top_index].y;
		trapezoid[0].bottom = screen_coords[bottom_index].y;
		trapezoid[0].left.screen_coord1 = screen_coords[top_index];
		trapezoid[0].left.screen_coord2 = screen_coords[middle_index];
		trapezoid[0].left.screen_depth1 = screen_depth[top_index];
		trapezoid[0].left.screen_depth2 = screen_depth[middle_index];
		trapezoid[0].left.varyings1 = varyings[top_index];
		trapezoid[0].left.varyings2 = varyings[middle_index];
		trapezoid[0].right.screen_coord1 = screen_coords[top_index];
		trapezoid[0].right.screen_coord2 = screen_coords[bottom_index];
		trapezoid[0].right.screen_depth1 = screen_depth[top_index];
		trapezoid[0].right.screen_depth2 = screen_depth[bottom_index];
		trapezoid[0].right.varyings1 = varyings[top_index];
		trapezoid[0].right.varyings2 = varyings[bottom_index];
		return 1;
	}

	// T  -  M
	//  \   /
	//    B
	if (screen_coords[top_index].y == screen_coords[middle_index].y && screen_coords[middle_index].y != screen_coords[bottom_index].y)
	{
		if (screen_coords[top_index].x > screen_coords[middle_index].x) { std::swap(top_index, middle_index); }

		trapezoid[0].top = screen_coords[top_index].y;
		trapezoid[0].bottom = screen_coords[bottom_index].y;
		trapezoid[0].left.screen_coord1 = screen_coords[top_index];
		trapezoid[0].left.screen_coord2 = screen_coords[bottom_index];
		trapezoid[0].left.screen_depth1 = screen_depth[top_index];
		trapezoid[0].left.screen_depth2 = screen_depth[bottom_index];
		trapezoid[0].left.varyings1 = varyings[top_index];
		trapezoid[0].left.varyings2 = varyings[bottom_index];
		trapezoid[0].right.screen_coord1 = screen_coords[middle_index];
		trapezoid[0].right.screen_coord2 = screen_coords[bottom_index];
		trapezoid[0].right.screen_depth1 = screen_depth[middle_index];
		trapezoid[0].right.screen_depth2 = screen_depth[bottom_index];
		trapezoid[0].right.varyings1 = varyings[middle_index];
		trapezoid[0].right.varyings2 = varyings[bottom_index];
		return 1;
	}

	// T \      / T
	// |  M or M  |
	// B /      \ B
	const float t = (screen_coords[middle_index].y - screen_coords[top_index].y) / (screen_coords[bottom_index].y - screen_coords[top_index].y);
	const float x = screen_coords[middle_index].x + (screen_coords[bottom_index].x - screen_coords[middle_index].x) * t;

	trapezoid[0].top = screen_coords[top_index].y;
	trapezoid[0].bottom = screen_coords[middle_index].y;
	trapezoid[1].top = screen_coords[middle_index].y;
	trapezoid[1].bottom = screen_coords[bottom_index].y;

	// T \ 
	// |  M
	// B / 
	if (screen_coords[middle_index].x > x)
	{
		// Triangle top - middle
		trapezoid[0].left.screen_coord1 = screen_coords[top_index];
		trapezoid[0].left.screen_coord2 = screen_coords[bottom_index];
		trapezoid[0].left.screen_depth1 = screen_depth[top_index];
		trapezoid[0].left.screen_depth2 = screen_depth[bottom_index];
		trapezoid[0].left.varyings1 = varyings[top_index];
		trapezoid[0].left.varyings2 = varyings[bottom_index];
		trapezoid[0].right.screen_coord1 = screen_coords[top_index];
		trapezoid[0].right.screen_coord2 = screen_coords[middle_index];
		trapezoid[0].right.screen_depth1 = screen_depth[top_index];
		trapezoid[0].right.screen_depth2 = screen_depth[middle_index];
		trapezoid[0].right.varyings1 = varyings[top_index];
		trapezoid[0].right.varyings2 = varyings[middle_index];

		// Triangle middle - bottom
		trapezoid[1].left.screen_coord1 = screen_coords[top_index];
		trapezoid[1].left.screen_coord2 = screen_coords[bottom_index];
		trapezoid[1].left.screen_depth1 = screen_depth[top_index];
		trapezoid[1].left.screen_depth2 = screen_depth[bottom_index];
		trapezoid[1].left.varyings1 = varyings[top_index];
		trapezoid[1].left.varyings2 = varyings[bottom_index];
		trapezoid[1].right.screen_coord1 = screen_coords[middle_index];
		trapezoid[1].right.screen_coord2 = screen_coords[bottom_index];
		trapezoid[1].right.screen_depth1 = screen_depth[middle_index];
		trapezoid[1].right.screen_depth2 = screen_depth[bottom_index];
		trapezoid[1].right.varyings1 = varyings[middle_index];
		trapezoid[1].right.varyings2 = varyings[bottom_index];
	}
	//  / T
	// M  |
	//  \ B
	else
	{
		// Triangle top - middle
		trapezoid[0].left.screen_coord1 = screen_coords[top_index];
		trapezoid[0].left.screen_coord2 = screen_coords[middle_index];
		trapezoid[0].left.screen_depth1 = screen_depth[top_index];
		trapezoid[0].left.screen_depth2 = screen_depth[middle_index];
		trapezoid[0].left.varyings1 = varyings[top_index];
		trapezoid[0].left.varyings2 = varyings[middle_index];
		trapezoid[0].right.screen_coord1 = screen_coords[top_index];
		trapezoid[0].right.screen_coord2 = screen_coords[bottom_index];
		trapezoid[0].right.screen_depth1 = screen_depth[top_index];
		trapezoid[0].right.screen_depth2 = screen_depth[bottom_index];
		trapezoid[0].right.varyings1 = varyings[top_index];
		trapezoid[0].right.varyings2 = varyings[bottom_index];

		// Triangle middle - bottom
		trapezoid[1].left.screen_coord1 = screen_coords[middle_index];
		trapezoid[1].left.screen_coord2 = screen_coords[bottom_index];
		trapezoid[1].left.screen_depth1 = screen_depth[middle_index];
		trapezoid[1].left.screen_depth2 = screen_depth[bottom_index];
		trapezoid[1].left.varyings1 = varyings[middle_index];
		trapezoid[1].left.varyings2 = varyings[bottom_index];
		trapezoid[1].right.screen_coord1 = screen_coords[top_index];
		trapezoid[1].right.screen_coord2 = screen_coords[bottom_index];
		trapezoid[1].right.screen_depth1 = screen_depth[top_index];
		trapezoid[1].right.screen_depth2 = screen_depth[bottom_index];
		trapezoid[1].right.varyings1 = varyings[top_index];
		trapezoid[1].right.varyings2 = varyings[bottom_index];
	}

	return 2;
}

static float InterpolateDepth_V1(const Trapezoid& trapezoid, float tx, float ty1, float ty2)
{
	const float depth0 = math::FloatLerp(trapezoid.left.screen_depth1, trapezoid.left.screen_depth2, ty1);
	const float depth1 = math::FloatLerp(trapezoid.right.screen_depth1, trapezoid.right.screen_depth2, ty2);
	const float depth2 = math::FloatLerp(depth0, depth1, tx);
	return depth2;
}

static void InterpolateVaryings_V1(const Trapezoid& trapezoid, void* dst_varyings, int32_t sizeof_varyings, float tx, float ty1, float ty2)
{
	const int32_t num_floats = sizeof_varyings / sizeof(float);
	const float* src_left1 = reinterpret_cast<const float*>(trapezoid.left.varyings1);
	const float* src_left2 = reinterpret_cast<const float*>(trapezoid.left.varyings2);
	const float* src_right1 = reinterpret_cast<const float*>(trapezoid.right.varyings1);
	const float* src_right2 = reinterpret_cast<const float*>(trapezoid.right.varyings2);
	float* dst = reinterpret_cast<float*>(dst_varyings);
	for (int32_t i = 0; i < num_floats; ++i)
	{
		const float src0 = math::FloatLerp(src_left1[i], src_left2[i], ty1);
		const float src1 = math::FloatLerp(src_right1[i], src_right2[i], ty2);
		dst[i] = math::FloatLerp(src0, src1, tx);
	}
}

struct BoundingBox
{
	int32_t min_x;
	int32_t min_y;
	int32_t max_x;
	int32_t max_y;
};

static BoundingBox MakeBoundingBox_V2(const math::Vector2 screen_coords[3], int32_t width, int32_t height)
{
	const math::Vector2 min = math::Vector2Min(math::Vector2Min(screen_coords[0], screen_coords[1]), screen_coords[2]);
	const math::Vector2 max = math::Vector2Max(math::Vector2Max(screen_coords[0], screen_coords[1]), screen_coords[2]);
	BoundingBox box;
	box.min_x = math::Max(math::FloorToInt(min.x), 0);
	box.min_y = math::Max(math::FloorToInt(min.y), 0);
	box.max_x = math::Min(math::CeilToInt(max.x), width);
	box.max_y = math::Min(math::CeilToInt(max.y), height);
	return box;
}

static math::Vector3 CalculateWeights_V2(const math::Vector2 screen_coords[3], const math::Vector2& point)
{
	const math::Vector2& a = screen_coords[0];
	const math::Vector2& b = screen_coords[1];
	const math::Vector2& c = screen_coords[2];
	const math::Vector2 ab = b - a;
	const math::Vector2 ac = c - a;
	const math::Vector2 ap = point - a;
	const float factor = 1.0f / (ab.x * ac.y - ab.y * ac.x);
	const float s = (ac.y * ap.x - ac.x * ap.y) * factor;
	const float t = (ab.x * ap.y - ab.y * ap.x) * factor;
	return math::Vector3(1.0f - s - t, s, t);
}

static float InterpolateDepth_V2(const float screen_depths[3], const math::Vector3& weights)
{
	const float depth0 = screen_depths[0] * weights.x;
	const float depth1 = screen_depths[1] * weights.y;
	const float depth2 = screen_depths[2] * weights.z;
	return depth0 + depth1 + depth2;
}

static void InterpolateVaryings_V2(void* src_varyings[3], void* dst_varyings, int32_t sizeof_varyings, const math::Vector3& weights, const float inv_w[3])
{
	const int32_t num_floats = sizeof_varyings / sizeof(float);
	const float* src0 = reinterpret_cast<const float*>(src_varyings[0]);
	const float* src1 = reinterpret_cast<const float*>(src_varyings[1]);
	const float* src2 = reinterpret_cast<const float*>(src_varyings[2]);
	float* dst = reinterpret_cast<float*>(dst_varyings);
	const float weight0 = inv_w[0] * weights.x;
	const float weight1 = inv_w[1] * weights.y;
	const float weight2 = inv_w[2] * weights.z;
	const float normalizer = 1.0f / (weight0 + weight1 + weight2);
	for (int32_t i = 0; i < num_floats; ++i)
	{
		const float sum = src0[i] * weight0 + src1[i] * weight1 + src2[i] * weight2;
		dst[i] = sum * normalizer;
	}
}

static bool IsBackFacing(const math::Vector3 ndc_coords[3])
{
	return false;
}

static math::Vector3 ViewportTransform(int32_t width, int32_t height, const math::Vector3& ndc_coords)
{
	const float x = (ndc_coords.x + 1.0f) * 0.5f * static_cast<float>(width);	// [-1, 1] -> [0, w]
	const float y = (1.0f - ndc_coords.y) * 0.5f * static_cast<float>(height);	// [-1, 1] -> [0, h]
	const float z = (ndc_coords.z + 1.0f) * 0.5f;								// [-1, 1] -> [0, 1]
	return math::Vector3(x, y, z);
}

static void DrawFragment(const FrameBuffer& frame_buffer, PipelineContext& context, int32_t index)
{
	// Execute pixel shader
	bool discard = false;
	math::Vector4 color = context.shader->PixelShader(context.shader_varyings, context.shader_constants, discard);

	if (discard)
	{
		return;
	}

	color = math::Vector4Saturate(color);

	// Perform blending
	if (context.enable_blend)
	{
		// out_color = src_color * src_alpha + dst_color * (1 - src_alpha)
		uint8_t dst_r = frame_buffer.pixel_buffer[index * 4 + 0];
		uint8_t dst_g = frame_buffer.pixel_buffer[index * 4 + 1];
		uint8_t dst_b = frame_buffer.pixel_buffer[index * 4 + 2];
		color.x = color.x * color.w + math::FloatFromUChar(dst_r) * (1.0f - color.w);
		color.y = color.y * color.w + math::FloatFromUChar(dst_g) * (1.0f - color.w);
		color.z = color.z * color.w + math::FloatFromUChar(dst_b) * (1.0f - color.w);
	}

	// Output merge
	frame_buffer.pixel_buffer[index * 4 + 0] = math::FloatToUChar(color.x); // red
	frame_buffer.pixel_buffer[index * 4 + 1] = math::FloatToUChar(color.y); // green
	frame_buffer.pixel_buffer[index * 4 + 2] = math::FloatToUChar(color.z); // blue
}

void rasterizer::RasterizeTriangle_V1(const FrameBuffer& frame_buffer, PipelineContext& context, const math::Vector4 clip_coords[3], void* varyings[3])
{
	// Perspective division
	math::Vector3 ndc_coords[3];
	for (int32_t i = 0; i < 3; ++i)
	{
		const math::Vector3 clip_coord = math::Vector3(clip_coords[i].x, clip_coords[i].y, clip_coords[i].z);
		ndc_coords[i] = clip_coord / clip_coords[i].w;
	}

	// Back-face culling
	const bool backface = IsBackFacing(ndc_coords);
	if (backface)
	{
		return;
	}

	// Inverse of w
	float inv_w[3];
	for (int32_t i = 0; i < 3; ++i)
	{
		inv_w[i] = 1.0f / clip_coords[i].w;
	}

	// Viewport mapping
	math::Vector2 screen_coords[3];
	float screen_depth[3];
	for (int32_t i = 0; i < 3; ++i)
	{
		const math::Vector3 viewport_coords = ViewportTransform(frame_buffer.width, frame_buffer.height, ndc_coords[i]);
		screen_coords[i] = math::Vector2(viewport_coords.x, viewport_coords.y);
		screen_depth[i] = ndc_coords[i].z;
	}

	Trapezoid trapezoids[2];
	const int32_t num_triangles = MakeTrapezoid_V1(trapezoids, screen_coords, screen_depth, varyings);

	for (int32_t i = 0; i < num_triangles; ++i)
	{
		const Trapezoid& trapezoid = trapezoids[i];

		const int32_t min_y = math::Max(math::FloorToInt(trapezoid.top + 0.5f), 0);
		const int32_t max_y = math::Min(math::CeilToInt(trapezoid.bottom - 0.5f), frame_buffer.height);

		const float delta_y1 = 1.0f / (trapezoid.left.screen_coord2.y - trapezoid.left.screen_coord1.y);
		const float delta_y2 = 1.0f / (trapezoid.right.screen_coord2.y - trapezoid.right.screen_coord1.y);
		for (int32_t y = min_y; y < max_y; ++y)
		{
			const float fy = static_cast<float>(y) + 0.5f;
			const float ty1 = (fy - trapezoid.left.screen_coord1.y) * delta_y1;
			const float ty2 = (fy - trapezoid.right.screen_coord1.y) * delta_y2;
			const float fx1 = math::FloatLerp(trapezoid.left.screen_coord1.x, trapezoid.left.screen_coord2.x, ty1);
			const float fx2 = math::FloatLerp(trapezoid.right.screen_coord1.x, trapezoid.right.screen_coord2.x, ty2);
			const int32_t min_x = math::Max(math::FloorToInt(fx1 + 0.5f), 0);
			const int32_t max_x = math::Min(math::CeilToInt(fx2 - 0.5f), frame_buffer.width);

			const float delta_x = 1.0f / (fx2 - fx1);
			for (int32_t x = min_x; x < max_x; ++x)
			{
				const float fx = static_cast<float>(x) + 0.5f;
				const float tx = (fx - fx1) * delta_x;
				const int32_t index = y * frame_buffer.width + x;
				const float depth = InterpolateDepth_V1(trapezoid, tx, ty1, ty2);
				// Depth test
				if (depth <= frame_buffer.depth_buffer[index])
				{
					InterpolateVaryings_V1(trapezoid, context.shader_varyings, context.sizeof_varyings, tx, ty1, ty2);
					DrawFragment(frame_buffer, context, index);
				}
			}
		}
	}
}

void rasterizer::RasterizeTriangle_V2(const FrameBuffer& frame_buffer, PipelineContext& context, const math::Vector4 clip_coords[3], void* varyings[3])
{
	// Perspective division
	math::Vector3 ndc_coords[3];
	for (int32_t i = 0; i < 3; ++i)
	{
		const math::Vector3 clip_coord = math::Vector3(clip_coords[i].x, clip_coords[i].y, clip_coords[i].z);
		ndc_coords[i] = clip_coord / clip_coords[i].w;
	}

	// Back-face culling
	const bool backface = IsBackFacing(ndc_coords);
	if (backface)
	{
		return;
	}

	// Inverse of w
	float inv_w[3];
	for (int32_t i = 0; i < 3; ++i)
	{
		inv_w[i] = 1.0f / clip_coords[i].w;
	}

	// Viewport mapping
	math::Vector2 screen_coords[3];
	float screen_depth[3];
	for (int32_t i = 0; i < 3; ++i)
	{
		const math::Vector3 viewport_coords = ViewportTransform(frame_buffer.width, frame_buffer.height, ndc_coords[i]);
		screen_coords[i] = math::Vector2(viewport_coords.x, viewport_coords.y);
		screen_depth[i] = ndc_coords[i].z;
	}

	const BoundingBox box = MakeBoundingBox_V2(screen_coords, frame_buffer.width, frame_buffer.height);

	for (int32_t x = box.min_x; x < box.max_x; ++x)
	{
		for (int32_t y = box.min_y; y < box.max_y; ++y)
		{
			const math::Vector2 point = math::Vector2(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f);
			const math::Vector3 weights = CalculateWeights_V2(screen_coords, point);
			if (0.0f < weights.x && 0.0f < weights.y && 0.0f < weights.z)
			{
				const int32_t index = y * frame_buffer.width + x;
				const float depth = InterpolateDepth_V2(screen_depth, weights);
				// Depth test
				if (depth <= frame_buffer.depth_buffer[index])
				{
					InterpolateVaryings_V2(varyings, context.shader_varyings, context.sizeof_varyings, weights, inv_w);
					DrawFragment(frame_buffer, context, index);
				}
			}
		}
	}
}
