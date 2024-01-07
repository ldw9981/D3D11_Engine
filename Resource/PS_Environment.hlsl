#include "Shared.hlsli"



float4 main(PS_INPUT_ENVIRONMENT input) : SV_Target
{
    return txEnvironment.Sample(samplerLinear, input.TexCoords);
}