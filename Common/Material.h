#pragma once
#include "D3DRenderManager.h"


using namespace Microsoft::WRL;

struct aiMaterial;


class MaterialTexture
{
public:
	MaterialTexture();
	~MaterialTexture();

	std::wstring m_FilePath;
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;	// ≈ÿΩ∫√≥ ∏Æº“Ω∫ ∫‰.
	void Create(const std::wstring& filePath);
};



class Material
{
public:
	Material();
	~Material();

	void TestMaterialPropery(const aiMaterial* pMaterial);
	void TestTextureProperty(const aiMaterial* pMaterial, std::vector<std::pair<aiTextureType, std::string>>& textureProperties);
	void Create(const aiMaterial* pMaterial);

	Math::Vector4 m_Color;	

	std::shared_ptr<MaterialTexture> m_pBaseColor;	// ≈ÿΩ∫√≥ ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pNormal;// ≥Î∏ª∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pSpecular;// Ω∫∆‰≈ß∑Ø∏  ∏Æº“Ω∫ ∫‰.
	std::shared_ptr<MaterialTexture> m_pEmissive;// ¿ÃπÃΩ√∫Í∏  ∏Æº“Ω∫ ∫‰.	
	std::shared_ptr<MaterialTexture> m_pOpacity;// ≈ı∏Ì∏  ∏Æº“Ω∫ ∫‰.	
	std::shared_ptr<MaterialTexture> m_pRoughness;	// ∞≈ƒ•±‚
	std::shared_ptr<MaterialTexture> m_pMetalness;	// ±›º”º∫	

	//void ApplyDeviceContext(ID3D11DeviceContext* deviceContext, CB_Marterial* cpuCbMaterial, ID3D11Buffer* gpuCbMarterial, ID3D11BlendState* alphaBlendState);
};

