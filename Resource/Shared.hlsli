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
TextureCube txIBL_Diffuse : register(t8);
TextureCube txIBL_Specular : register(t9);
Texture2D txIBL_SpecularBRDF_LUT : register(t10);
Texture2D txShadow : register(t11);

SamplerState samplerLinear : register(s0);
SamplerState samplerClamp : register(s1);
SamplerState samplerBorder : register(s2)
{
    AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    BorderColor = (float4)1; //uvw가 0~1사이가 아닌경우 테두리 색상을 지정한다.
};

cbuffer TransformW : register(b0)
{
    matrix World;
}

cbuffer TransformVP : register(b1)
{
    matrix View;
    matrix Projection;
    matrix ShadowView;         
    matrix ShadowProjection;   
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
    int padMaterial;
}

cbuffer MatrixPalette : register(b4)
{
    matrix MatrixPaletteArray[128];
}

cbuffer Global : register(b5)
{
    int UseIBL;                 // 4  
    float AmbientOcclusion ;    // 4
    int UseGammaCorrection;
    float Gamma;
};

cbuffer MaterialOverride : register(b6)
{
    int UseMarterialOverride;
    float3 BaseColorOverride; //16
    float MetalnessOverride;
    float RoughnessOverride; 
    float2 MarterialOverridePad; // 16
};



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
        
    float4 PositionShadow : TEXCOORD1;
};

struct PS_INPUT_ENVIRONMENT
{
    float4 PositionProj : SV_POSITION;
    float3 TexCoords : TEXCOORD0;
};