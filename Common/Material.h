#pragma once
#include "D3DRenderManager.h"
#include "TextureResource.h"

using namespace Microsoft::WRL;

struct aiMaterial;




class Material
{
public:
	Material();
	~Material();

	void TestMaterialPropery(const aiMaterial* pMaterial);
	void TestTextureProperty(const aiMaterial* pMaterial, std::vector<std::pair<aiTextureType, std::string>>& textureProperties);
	void Create(const aiMaterial* pMaterial);

	std::shared_ptr<TextureResource> m_pBaseColor;	// ≈ÿΩ∫√≥ ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<TextureResource> m_pNormal;// ≥Î∏ª∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<TextureResource> m_pSpecular;// Ω∫∆‰≈ß∑Ø∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<TextureResource> m_pEmissive;// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	std::shared_ptr<TextureResource> m_pOpacity;// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.	
	std::shared_ptr<TextureResource> m_pMetalness;	// ±›º”º∫	
	std::shared_ptr<TextureResource> m_pRoughness;	// ∞≈ƒ•±‚
	

	//void ApplyDeviceContext(ID3D11DeviceContext* deviceContext, CB_Marterial* cpuCbMaterial, ID3D11Buffer* gpuCbMarterial, ID3D11BlendState* alphaBlendState);
};

