#pragma once

#include "Math/VectorRegister.h"
#include "Math/MatrixRegister.h"

namespace sr
{
	namespace shader
	{
		class IVertexShader
		{
		public:
			virtual void SetConstantStream(Archive& ar) {}
			virtual Archive VS(Archive& ar) = 0;
		};
	}
}
