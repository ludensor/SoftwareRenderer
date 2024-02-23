#pragma once

#include "Math/VectorRegister.h"
#include "Math/MatrixRegister.h"

namespace sr
{
	namespace shader
	{
		class IPixelShader
		{
		public:
			virtual void SetConstantStream(Archive& ar) {}
			virtual math::VectorRegister PS(Archive& ar) = 0;
		};
	}
}
