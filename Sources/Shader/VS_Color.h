#pragma once

#include "VertexShaderInterface.h"

namespace sr
{
	namespace shader
	{
		class VertexShader_Color : public IVertexShader
		{
		public:
			virtual void SetConstantStream(Archive& ar) final;
			virtual Archive VS(Archive& ar) final;

		private:
			math::MatrixRegister world_matrix_;
			math::MatrixRegister view_matrix_;
			math::MatrixRegister projection_matrix_;
		};
	}
}
