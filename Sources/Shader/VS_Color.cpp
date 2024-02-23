#include "VS_Color.h"

void sr::shader::VertexShader_Color::SetConstantStream(Archive& ar)
{
	ar >> world_matrix_ >> view_matrix_ >> projection_matrix_;
}

sr::Archive sr::shader::VertexShader_Color::VS(Archive& ar)
{
	Archive output;

	math::VectorRegister position;
	math::VectorRegister color;
	ar >> position >> color;

	return Archive();
}

