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

	std::shared_ptr<TextureResource> m_pBaseColor;	// �ؽ�ó ���ҽ� ��.
	std::shared_ptr<TextureResource> m_pNormal;// �븻�� ���ҽ� ��.
	std::shared_ptr<TextureResource> m_pSpecular;// ����ŧ���� ���ҽ� ��.
	std::shared_ptr<TextureResource> m_pEmissive;// �̹̽ú�� ���ҽ� ��.	
	std::shared_ptr<TextureResource> m_pOpacity;// ����� ���ҽ� ��.	
	std::shared_ptr<TextureResource> m_pMetalness;	// �ݼӼ�	
	std::shared_ptr<TextureResource> m_pRoughness;	// ��ĥ��
	

	//void ApplyDeviceContext(ID3D11DeviceContext* deviceContext, CB_Marterial* cpuCbMaterial, ID3D11Buffer* gpuCbMarterial, ID3D11BlendState* alphaBlendState);
};

