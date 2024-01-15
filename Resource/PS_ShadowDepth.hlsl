#include "Shared.hlsli"
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float depth = input.PositionProj.z / input.PositionProj.w;
    return float4(depth.xxx, 1);
}