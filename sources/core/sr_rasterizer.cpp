#include "sr_pch.h"
#include "core/sr_rasterizer.h"
#include "shaders/sr_shader_interface.h"

void Rasterizer::RasterizeTriangle_V1(const FrameBuffer& frame_buffer, PipelineContext& context, const math::Vector4 clip_coords[3], void* varyings[3])
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
	const int32_t num_triangles = SeparateTriangle_V1(trapezoids, screen_coords, varyings);

	for (int32_t i = 0; i < num_triangles; ++i)
	{
		const Trapezoid& trapezoid = trapezoids[i];

		const int32_t y1 = math::Clamp(static_cast<int32_t>(trapezoid.top + 0.5f), 0, frame_buffer.height);
		const int32_t y2 = math::Clamp(static_cast<int32_t>(trapezoid.bottom + 0.5f), 0, frame_buffer.height);

		const float delta_y1 = 1.0f / (trapezoid.left.screen_coord2->y - trapezoid.left.screen_coord1->y);
		const float delta_y2 = 1.0f / (trapezoid.right.screen_coord2->y - trapezoid.right.screen_coord1->y);
		for (int32_t y = y1; y < y2; ++y)
		{
			const float fy = static_cast<float>(y) + 0.5f;
			const float ty1 = (fy - trapezoid.left.screen_coord1->y) * delta_y1;
			const float ty2 = (fy - trapezoid.right.screen_coord1->y) * delta_y2;
			const float fx1 = math::FloatLerp(trapezoid.left.screen_coord1->x, trapezoid.left.screen_coord2->x, ty1);
			const float fx2 = math::FloatLerp(trapezoid.right.screen_coord1->x, trapezoid.right.screen_coord2->x, ty2);
			const int32_t x1 = math::Clamp(static_cast<int32_t>(fx1 + 0.5f), 0, frame_buffer.width);
			const int32_t x2 = math::Clamp(static_cast<int32_t>(fx2 + 0.5f), 0, frame_buffer.width);

			const float delta_x = 1.0f / (fx2 - fx1);
			for (int32_t x = x1; x < x2; ++x)
			{
				const float fx = static_cast<float>(x);
				const float tx = (fx - fx1) * delta_x;
				const int32_t index = y * frame_buffer.width + x;
				InterpolateVaryings_V1(trapezoid, context.shader_varyings, context.sizeof_varyings, tx, ty1, ty2);
				DrawFragment(frame_buffer, context, index);
			}
		}
	}
}

bool Rasterizer::IsBackFacing(const math::Vector3 ndc_coords[3])
{
	return false;
}

math::Vector3 Rasterizer::ViewportTransform(int32_t width, int32_t height, const math::Vector3& ndc_coords)
{
	const float x = (ndc_coords.x + 1.0f) * 0.5f * static_cast<float>(width);	// [-1, 1] -> [0, w]
	const float y = (1.0f - ndc_coords.y) * 0.5f * static_cast<float>(height);	// [-1, 1] -> [0, h]
	const float z = (ndc_coords.z + 1.0f) * 0.5f;								// [-1, 1] -> [0, 1]
	return math::Vector3(x, y, z);
}

int32_t Rasterizer::SeparateTriangle_V1(Trapezoid trapezoid[2], math::Vector2 screen_coords[3], void* varyings[3])
{
	if (screen_coords[0].y > screen_coords[1].y)
	{
		std::swap(screen_coords[0], screen_coords[1]);
		std::swap(varyings[0], varyings[1]);
	}
	if (screen_coords[0].y > screen_coords[2].y)
	{
		std::swap(screen_coords[0], screen_coords[2]);
		std::swap(varyings[0], varyings[2]);
	}
	if (screen_coords[1].y > screen_coords[2].y)
	{
		std::swap(screen_coords[1], screen_coords[2]);
		std::swap(varyings[1], varyings[2]);
	}

	//    T
	//  /   \
	// M  -  B
	if (screen_coords[0].y != screen_coords[1].y && screen_coords[1].y == screen_coords[2].y)
	{
		if (screen_coords[1].x > screen_coords[2].x)
		{
			std::swap(screen_coords[1], screen_coords[2]);
			std::swap(varyings[1], varyings[2]);
		}

		trapezoid[0].top = screen_coords[0].y;
		trapezoid[0].bottom = screen_coords[2].y;
		trapezoid[0].left.screen_coord1 = &screen_coords[0];
		trapezoid[0].left.screen_coord2 = &screen_coords[1];
		trapezoid[0].left.varyings1 = varyings[0];
		trapezoid[0].left.varyings2 = varyings[1];
		trapezoid[0].right.screen_coord1 = &screen_coords[0];
		trapezoid[0].right.screen_coord2 = &screen_coords[2];
		trapezoid[0].right.varyings1 = varyings[0];
		trapezoid[0].right.varyings2 = varyings[2];
		return 1;
	}

	// T  -  M
	//  \   /
	//    B
	if (screen_coords[0].y == screen_coords[1].y && screen_coords[1].y != screen_coords[2].y)
	{
		if (screen_coords[0].x > screen_coords[1].x)
		{
			std::swap(screen_coords[0], screen_coords[1]);
			std::swap(varyings[0], varyings[1]);
		}

		trapezoid[0].top = screen_coords[0].y;
		trapezoid[0].bottom = screen_coords[2].y;
		trapezoid[0].left.screen_coord1 = &screen_coords[0];
		trapezoid[0].left.screen_coord2 = &screen_coords[2];
		trapezoid[0].left.varyings1 = varyings[0];
		trapezoid[0].left.varyings2 = varyings[2];
		trapezoid[0].right.screen_coord1 = &screen_coords[1];
		trapezoid[0].right.screen_coord2 = &screen_coords[2];
		trapezoid[0].right.varyings1 = varyings[1];
		trapezoid[0].right.varyings2 = varyings[2];
		return 1;
	}

	// T \      / T
	// |  M or M  |
	// B /      \ B
	if (screen_coords[0].y != screen_coords[1].y && screen_coords[1].y != screen_coords[2].y)
	{
		const float t = (screen_coords[1].y - screen_coords[0].y) / (screen_coords[2].y - screen_coords[0].y);
		const float x = screen_coords[1].x + (screen_coords[2].x - screen_coords[1].x) * t;

		trapezoid[0].top = screen_coords[0].y;
		trapezoid[0].bottom = screen_coords[1].y;
		trapezoid[1].top = screen_coords[1].y;
		trapezoid[1].bottom = screen_coords[2].y;

		// T \ 
		// |  M
		// B / 
		if (screen_coords[1].x > x)
		{
			// Triangle top - middle
			trapezoid[0].left.screen_coord1 = &screen_coords[0];
			trapezoid[0].left.screen_coord2 = &screen_coords[2];
			trapezoid[0].left.varyings1 = varyings[0];
			trapezoid[0].left.varyings2 = varyings[2];
			trapezoid[0].right.screen_coord1 = &screen_coords[0];
			trapezoid[0].right.screen_coord2 = &screen_coords[1];
			trapezoid[0].right.varyings1 = varyings[0];
			trapezoid[0].right.varyings2 = varyings[1];

			// Triangle middle - bottom
			trapezoid[1].left.screen_coord1 = &screen_coords[0];
			trapezoid[1].left.screen_coord2 = &screen_coords[2];
			trapezoid[1].left.varyings1 = varyings[0];
			trapezoid[1].left.varyings2 = varyings[2];
			trapezoid[1].right.screen_coord1 = &screen_coords[1];
			trapezoid[1].right.screen_coord2 = &screen_coords[2];
			trapezoid[1].right.varyings1 = varyings[1];
			trapezoid[1].right.varyings2 = varyings[2];
		}
		//  / T
		// M  |
		//  \ B
		else
		{
			// Triangle top - middle
			trapezoid[0].left.screen_coord1 = &screen_coords[0];
			trapezoid[0].left.screen_coord2 = &screen_coords[1];
			trapezoid[0].left.varyings1 = varyings[0];
			trapezoid[0].left.varyings2 = varyings[1];
			trapezoid[0].right.screen_coord1 = &screen_coords[0];
			trapezoid[0].right.screen_coord2 = &screen_coords[2];
			trapezoid[0].right.varyings1 = varyings[0];
			trapezoid[0].right.varyings2 = varyings[2];

			// Triangle middle - bottom
			trapezoid[1].left.screen_coord1 = &screen_coords[1];
			trapezoid[1].left.screen_coord2 = &screen_coords[2];
			trapezoid[1].left.varyings1 = varyings[1];
			trapezoid[1].left.varyings2 = varyings[2];
			trapezoid[1].right.screen_coord1 = &screen_coords[0];
			trapezoid[1].right.screen_coord2 = &screen_coords[2];
			trapezoid[1].right.varyings1 = varyings[0];
			trapezoid[1].right.varyings2 = varyings[2];
		}

		return 2;
	}

	return 0;
}

void Rasterizer::InterpolateVaryings_V1(const Trapezoid& trapezoid, void* dst_varyings, int32_t sizeof_varyings, float tx, float ty1, float ty2)
{
	const int32_t num_floats = sizeof_varyings / sizeof(float);
	const float* src_left1 = reinterpret_cast<const float*>(trapezoid.left.varyings1);
	const float* src_left2 = reinterpret_cast<const float*>(trapezoid.left.varyings2);
	const float* src_right1 = reinterpret_cast<const float*>(trapezoid.right.varyings1);
	const float* src_right2 = reinterpret_cast<const float*>(trapezoid.right.varyings2);
	float* dst = reinterpret_cast<float*>(dst_varyings);

	for (int32_t i = 0; i < num_floats; ++i)
	{
		const float left = math::FloatLerp(src_left1[i], src_left2[i], ty1);
		const float right = math::FloatLerp(src_right1[i], src_right2[i], ty2);
		dst[i] = math::FloatLerp(left, right, tx);
	}
}

void Rasterizer::DrawFragment(const FrameBuffer& frame_buffer, PipelineContext& context, int32_t index)
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

	frame_buffer.pixel_buffer[index * 4 + 0] = math::FloatToUChar(color.x); // red
	frame_buffer.pixel_buffer[index * 4 + 1] = math::FloatToUChar(color.y); // green
	frame_buffer.pixel_buffer[index * 4 + 2] = math::FloatToUChar(color.z); // blue
}
