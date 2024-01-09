#include "Shared.hlsli"


static const float PI = 3.141592;
static const float Epsilon = 0.00001;

// 비금속 프레넬 계수
static const float3 Fdielectric = 0.04;



// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor. 최소값 F0 , 최대값은 1.0,1.0,1.0
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels()
{
    uint width, height, levels;
    txIBL_Specular.GetDimensions(0, width, height, levels);
    return levels;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
/*
	if문에서 함수를 호출하는 작업은 병렬처리에 방해가 되므로 if문에서의 호출은 피해야 해야한다.	
    텍스처사용 여부에 따라 각각 셰이더를 다른 셰이더로 컴파일하면 최적화가 되지만 
    입문자 입장에서는 관리하기 어려우므로 하나의 셰이더 파일로 처리한다.
*/
float4 main(PS_INPUT input) : SV_Target
{
	float3 vNormal = normalize(input.NormalWorld);
	float3 vTangent = normalize(input.TangentWorld);
	float3 vBiTanget = normalize(input.BiTangentWorld);
	float Opacity = 1.0f;
	
	if (UseNormalMap)
	{
        float3 vNormalTangentSpace = txNormal.Sample(samplerLinear, input.TexCoord).rgb * 2.0f - 1.0f;
		float3x3 WorldTransform = float3x3(vTangent, vBiTanget, vNormal);
		vNormal = mul(vNormalTangentSpace, WorldTransform);
		vNormal = normalize(vNormal);
	}
	
	// Sample input textures to get shading model params.
    float3 albedo = txBaseColor.Sample(samplerLinear, input.TexCoord).rgb;
    float metalness = 0.0f;
	if (UseMetalnessMap)
    {
        metalness = txMetalness.Sample(samplerLinear, input.TexCoord).r;
    }    
	
    float roughness = 0.0f;
	if (UseRoughnessMap)
    {
        roughness = txRoughness.Sample(samplerLinear, input.TexCoord).r;
    }   
    float3 emissive = 0.0f;
    if (UseEmissiveMap)
    {
        emissive = txEmissive.Sample(samplerLinear, input.TexCoord).rgb;
    }
    if (UseOpacityMap)
    {	
        Opacity = txOpacity.Sample(samplerLinear, input.TexCoord).a;
    }
	
	// Outgoing light direction (vector from world-space fragment position to the "eye").
	float3 Lo = normalize(EyePosition - input.PositionWorld);

	// Get current fragment's normal and transform to world space.
    float3 N = normalize(2.0 * txNormal.Sample(samplerLinear, input.TexCoord).rgb - 1.0);
    //N = normalize(mul(input.TangentBasis, N));
    N = vNormal;

	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));

	// Specular reflection vector.
	float3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	float3 F0 = lerp(Fdielectric, albedo, metalness);
	// Direct lighting calculation for analytical lights.
	float3 directLighting = 0.0;
	
	float3 Li = -LightDirection; 
	float3 Lradiance = LightRadiance;

	// Half-vector between Li and Lo.
	float3 Lh = normalize(Li + Lo);

	// Calculate angles between surface normal and various light vectors.
	float cosLi = max(0.0, dot(N, Li));
	float cosLh = max(0.0, dot(N, Lh));

	// Calculate Fresnel term for direct lighting. 
	float3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));  //최소값 F0 , 최대값은 1.0,1.0,1.0  
	
	// Calculate normal distribution for specular BRDF.
	float D = ndfGGX(cosLh, max(0.01, roughness));		// 러프니스 0 이되면 값이0이 되므로 0이면 최소값사용
	// Calculate geometric attenuation for specular BRDF.
	float G = gaSchlickGGX(cosLi, cosLo, roughness);
			
	// 금속성이 1이면 침투가 없으므로 표면산란이 일어나지 않아 표면산란의 색상은 0,0,0 으로 처리
	// 금속성이 0이면 반사의 나머지 비율만큼 표면산란의 색상 표현이 가능하다.
	// 금속성은 1픽셀에 해당되는 미세 표면의 분포를 의미하므로 보간을 상요하여 중간값을 처리한다.
	float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

	// Lambert diffuse BRDF.
	// We don't scale by 1/PI for lighting & material units to be more convenient.
	// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
	float3 diffuseBRDF = kd * albedo / PI;

	// Cook-Torrance specular microfacet BRDF.
	float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);  
	
	// Total contribution for this light.
	directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
	
	float3 ambientLighting = 0;		
    if (UseIBL)
    {
		// Sample diffuse irradiance at normal direction.
        float3 irradiance = txIBL_Diffuse.Sample(samplerLinear, N).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
        float3 F = fresnelSchlick(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
        float3 kd = lerp(1.0 - F, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
        float3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
        uint specularTextureLevels = querySpecularTextureLevels();
        float3 specularIrradiance = txIBL_Specular.SampleLevel(samplerLinear, Lr, roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
        float2 specularBRDF = txIBL_SpecularBRDF_LUT.Sample(samplerSpecularBRDF, float2(cosLo, roughness)).rg;

		// Total specular IBL contribution.
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        ambientLighting = diffuseIBL + specularIBL;
    }
	
	
    float3 final = directLighting + ambientLighting + emissive ;
    float3 GammaCorrect = pow(final, float(1.0 / 2.2).rrr);
    float3 output = GammaCorrect;

    return float4(output, Opacity);
}
