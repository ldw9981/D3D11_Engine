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
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;	// �ؽ�ó ���ҽ� ��.
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

	std::shared_ptr<MaterialTexture> m_pBaseColor;	// �ؽ�ó ���ҽ� ��.
	std::shared_ptr<MaterialTexture> m_pNormal;// �븻�� ���ҽ� ��.
	std::shared_ptr<MaterialTexture> m_pSpecular;// ����ŧ���� ���ҽ� ��.
	std::shared_ptr<MaterialTexture> m_pEmissive;// �̹̽ú�� ���ҽ� ��.	
	std::shared_ptr<MaterialTexture> m_pOpacity;// ����� ���ҽ� ��.	
	std::shared_ptr<MaterialTexture> m_pRoughness;	// ��ĥ��
	std::shared_ptr<MaterialTexture> m_pMetalness;	// �ݼӼ�	

	//void ApplyDeviceContext(ID3D11DeviceContext* deviceContext, CB_Marterial* cpuCbMaterial, ID3D11Buffer* gpuCbMarterial, ID3D11BlendState* alphaBlendState);
};

