#pragma once

#include "PixelShaderInterface.h"

namespace sr
{
	namespace shader
	{
		class PixelShader_Color : public IPixelShader
		{
		public:
			virtual math::VectorRegister PS(Archive& ar) final;
		};
	}
}
