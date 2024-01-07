#include "Shared.hlsli"



PS_INPUT_ENVIRONMENT main(VS_INPUT input)
{
    PS_INPUT_ENVIRONMENT output = (PS_INPUT_ENVIRONMENT) 0;
    float4 pos = mul(input.PositionModel, World);
    output.TexCoords = pos.xyz;  
    float3 pos3 = mul(pos.xyz,(float3x3)View); // 역행렬에서 위치는 적용안함
    pos = mul(float4(pos3, 1.0f), Projection);
    output.PositionProj = pos;    
    return output;
}