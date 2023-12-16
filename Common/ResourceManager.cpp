#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "Helper.h"

ResourceManager* ResourceManager::Instance = nullptr;

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ResourceManager::CreateTexture(std::wstring filePath)
{
	auto it = m_TextureMap.find(filePath);
	if (it != m_TextureMap.end())
	{
		return it->second;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureRV;	
	HR_T(CreateTextureFromFile(D3DRenderManager::m_pDevice, filePath.c_str(), &pTextureRV));
	return pTextureRV;
}

ResourceManager::ResourceManager()
{
	Instance = this;
}

ResourceManager::~ResourceManager()
{
	
}



std::shared_ptr<Material> ResourceManager::CreateMaterial(std::string key, aiMaterial* pAiMaterial)
{
	// Ű�� �̹� ������� ���͸����� �ִ��� ã�´�.
	auto it = m_MaterialMap.find(key);
	if (it != m_MaterialMap.end())
	{
		std::shared_ptr<Material> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			 return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			 m_MaterialMap.erase(it);
			 // �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}
	std::shared_ptr<Material> pMaterial = std::make_shared<Material>();
	pMaterial->Create(D3DRenderManager::m_pDevice,pAiMaterial);
	m_MaterialMap[key] = pMaterial;
	return pMaterial;
}


