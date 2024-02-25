#include "Shader.h"

sr::PixelData sr::ShaderColor::VS(const VertexData& input)
{
    return PixelData();
}

DirectX::XMVECTOR sr::ShaderColor::PS(const PixelData& input)
{
    return DirectX::XMVECTOR();
}

