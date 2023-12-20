#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "Helper.h"
#include "Material.h"
#include "SkeletalMeshResource.h"
#include "StaticMeshResource.h"


ResourceManager* ResourceManager::Instance = nullptr;

std::shared_ptr<MaterialTexture> ResourceManager::CreateMaterialTexture(std::wstring filePath)
{	
	auto it = m_MaterialTextureMap.find(filePath);
	if (it != m_MaterialTextureMap.end())
	{
		std::shared_ptr<MaterialTexture> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			m_MaterialTextureMap.erase(it);
			// �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}

	std::shared_ptr<MaterialTexture> pTextureRV = std::make_shared<MaterialTexture>();
	pTextureRV->Create(filePath);
	m_MaterialTextureMap[filePath] = pTextureRV;	
	return pTextureRV;
}


ResourceManager::ResourceManager()
{
	assert(Instance == nullptr);
	Instance = this;
}

ResourceManager::~ResourceManager()
{
	
}


std::shared_ptr<SkeletalMeshSceneResource> ResourceManager::CreateSkeletalMeshSceneResource(std::string filePath)
{
	// Ű�� �̹� ������� ���͸����� �ִ��� ã�´�.
	std::string key = filePath;
	auto it = m_SkeletalMeshSceneMap.find(key);
	if (it != m_SkeletalMeshSceneMap.end())
	{
		std::shared_ptr<SkeletalMeshSceneResource> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			m_SkeletalMeshSceneMap.erase(it);
			// �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}

	std::shared_ptr<SkeletalMeshSceneResource> pSceneResource = std::make_shared<SkeletalMeshSceneResource>();
	pSceneResource->Create(filePath.c_str());
	m_SkeletalMeshSceneMap[key] = pSceneResource;
	return pSceneResource;
}

std::shared_ptr<StaticMeshSceneResource> ResourceManager::CreateStaticMeshSceneResource(std::string filePath)
{
	// Ű�� �̹� ������� ���͸����� �ִ��� ã�´�.
	std::string key = filePath;
	auto it = m_StaticMeshSceneMap.find(key);
	if (it != m_StaticMeshSceneMap.end())
	{
		std::shared_ptr<StaticMeshSceneResource> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			m_StaticMeshSceneMap.erase(it);
			// �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}

	std::shared_ptr<StaticMeshSceneResource> pSceneResource = std::make_shared<StaticMeshSceneResource>();
	pSceneResource->Create(filePath.c_str());
	m_StaticMeshSceneMap[key] = pSceneResource;
	return pSceneResource;
}

