#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"

ResourceManager* ResourceManager::Instance = nullptr;

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


