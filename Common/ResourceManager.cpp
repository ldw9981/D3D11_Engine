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
	// 키로 이미 만들어진 머터리얼이 있는지 찾는다.
	auto it = m_MaterialMap.find(key);
	if (it != m_MaterialMap.end())
	{
		std::shared_ptr<Material> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이라 메모리가 아직 살아있다면 resourcePtr를 리턴한다.
		{
			 return resourcePtr;
		}
		else  //UseCount가 0이라면 메모리가 이미 해제되었으므로 맵에서 제거한다.
		{
			 m_MaterialMap.erase(it);
			 // 리턴하지 않고 아래에서 새로 만들어서 리턴한다.
		}
	}
	std::shared_ptr<Material> pMaterial = std::make_shared<Material>();
	pMaterial->Create(D3DRenderManager::m_pDevice,pAiMaterial);
	m_MaterialMap[key] = pMaterial;
	return pMaterial;
}


