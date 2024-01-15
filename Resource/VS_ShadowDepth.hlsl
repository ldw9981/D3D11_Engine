#include "Shared.hlsli"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    float4 pos = input.PositionModel;

    float4x4 matWorld;
#ifdef VERTEX_SKINNING   
    matWorld = mul(input.BlendWeights.x, MatrixPaletteArray[input.BlendIndices.x]);
    matWorld += mul(input.BlendWeights.y, MatrixPaletteArray[input.BlendIndices.y]);
    matWorld += mul(input.BlendWeights.z, MatrixPaletteArray[input.BlendIndices.z]);
    matWorld += mul(input.BlendWeights.w, MatrixPaletteArray[input.BlendIndices.w]);
#else
    matWorld = World;
#endif    
    pos = mul(pos, matWorld);
    output.PositionWorld = pos.xyz;    
    
    pos = mul(pos, ShadowView);
    pos = mul(pos, ShadowProjection);
    output.PositionProj = pos;    
    output.TexCoord = input.TexCoord;    
#ifndef NORMAL_OTHER   
    output.NormalWorld = normalize(mul(input.NormalModel, (float3x3) matWorld)); // scale 있을수 있으므로 normalize필요
    output.TangentWorld = normalize(mul(input.TangentModel, (float3x3) matWorld)); // scale 있을수 있으므로 normalize필요
    output.BiTangentWorld = normalize(mul(input.BiTangent, (float3x3) matWorld)); // scale 있을수 있으므로 normalize필요
 #else     
    float3x3 TBN = float3x3(input.TangentModel, input.BiTangent, input.NormalModel);
    output.TangentBasis = mul((float3x3) matWorld, transpose(TBN));
#endif
    return output;
}