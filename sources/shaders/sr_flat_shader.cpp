#include "sr_pch.h"
#include "shaders/sr_flat_shader.h"

math::Vector4 FlatShader::VertexShader(void* varyings, const void* attributes, const void* constants)
{
	const FlatVertexData* input = reinterpret_cast<const FlatVertexData*>(attributes);
	const FlatConstantData* uniform = reinterpret_cast<const FlatConstantData*>(constants);
	FlatVertexData* out = reinterpret_cast<FlatVertexData*>(varyings);

	math::Vector4 out_position = input->position * uniform->world_matrix;
	out_position = out_position * uniform->view_matrix;
	out_position = out_position * uniform->projection_matrix;
	out->color = input->color;
	return out->position;
}

math::Vector4 FlatShader::PixelShader(const void* varyings, const void* constants, bool& discard)
{
	const FlatVertexData* input = reinterpret_cast<const FlatVertexData*>(varyings);
	return input->color;
}
