#include "ColorShader.h"

#include <assert.h>

void sr::ColorShader::SetVertexShaderConstantBuffer(void* buffer, uint32_t buffer_size)
{
	memcpy(&buffer_, buffer, buffer_size);
}

sr::PixelShaderData sr::ColorShader::VertexShader(VertexShaderData input)
{
	ColorPixelShaderData output;

	ColorVertexShaderData* color_input = dynamic_cast<ColorVertexShaderData*>(&input);
	assert(color_input);
	output.position = sr::math::Vector4Multiplay(color_input->position, buffer_.world_matrix);
	output.position = sr::math::Vector4Multiplay(output.position, buffer_.view_matrix);
	output.position = sr::math::Vector4Multiplay(output.position, buffer_.projection_matrix);
	output.color = color_input->color;

	return output;
}

sr::Vector4 sr::ColorShader::PixelShader(PixelShaderData input)
{
	ColorPixelShaderData* color_input = dynamic_cast<ColorPixelShaderData*>(&input);
	assert(color_input);

	return color_input->color;
}

