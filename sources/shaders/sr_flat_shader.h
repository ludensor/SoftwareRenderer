#pragma once

#include "shaders/sr_shader_interface.h"

struct FlatAttributeData
{
	math::Vector3 position;
	math::Vector4 color;
};

struct FlatVertexData
{
	math::Vector4 position;
	math::Vector4 color;
};

struct FlatConstantData
{
	math::Matrix4x4 world_matrix;
	math::Matrix4x4 view_matrix;
	math::Matrix4x4 projection_matrix;
};

class FlatShader final : public IShader
{
public:
	virtual math::Vector4 VertexShader(void* varyings, const void* attributes, const void* constants) override final;
	virtual math::Vector4 PixelShader(const void* varyings, const void* constants, bool& discard) override final;
};
