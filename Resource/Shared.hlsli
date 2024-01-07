//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txBaseColor : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);
Texture2D txMetalness : register(t5);
Texture2D txRoughness : register(t6);

TextureCube txEnvironment : register(t7);
TextureCube txIBL_Irradiance : register(t9);
TextureCube txIBL_Specular : register(t8);
Texture2D txIBL_SpecularBRDF_LUT : register(t10);


SamplerState samplerLinear : register(s0);
SamplerState samplerSpecularBRDF : register(s1);

cbuffer TransformW : register(b0)
{
    matrix World;
}

cbuffer TransformVP : register(b1)
{
    matrix View;
    matrix Projection;
}


cbuffer DirectionLight : register(b2)
{
    float3 LightDirection;
    float DirectionLightPad0;
    float3 LightRadiance;
    float DirectionLightPad1;
    float3 EyePosition;
    float DirectionLightPad2;
}

cbuffer Material : register(b3)
{
    int UseBaseColorMap;
    int UseNormalMap;
    int UseSpecularMap;
    int UseEmissiveMap;
    int UseOpacityMap;
    int UseMetalnessMap;
    int UseRoughnessMap;
    int UseIBL;
}

cbuffer MatrixPalette : register(b4)
{
    matrix MatrixPaletteArray[128];
}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 PositionModel : POSITION;    
    float2 TexCoord : TEXCOORD0;
    float3 NormalModel : NORMAL;
    float3 TangentModel : TANGENT; 
    float3 BiTangent : BITANGENT;
#ifdef VERTEX_SKINNING 
    int4 BlendIndices : BLENDINDICES;
    float4 BlendWeights : BLENDWEIGHTS;
#endif
};

struct PS_INPUT
{
    float4 PositionProj : SV_POSITION;   
    float3 PositionWorld : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3x3 TangentBasis : TBASIS;
    
    float3 NormalWorld : NORMAL;
    float3 TangentWorld : TANGENT;
    float3 BiTangentWorld : BITANGENT;
};

struct PS_INPUT_ENVIRONMENT
{
    float4 PositionProj : SV_POSITION;
    float3 TexCoords : TEXCOORD0;
};