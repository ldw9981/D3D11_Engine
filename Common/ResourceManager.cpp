#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "Helper.h"
#include "Material.h"
#include "SkeletalMeshResource.h"
#include "StaticMeshResource.h"
#include "Animation.h"
#include "TimeSystem.h"

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

	std::filesystem::path path = filePath;
	if (!std::filesystem::exists(path)) {
		LOG_MESSAGEA("Erorr file not found: %s", path.string().c_str());
		return nullptr;
	};

	std::shared_ptr<MaterialTexture> pTextureRV = std::make_shared<MaterialTexture>();
	pTextureRV->Create(filePath);
	m_MaterialTextureMap[filePath] = pTextureRV;	
	return pTextureRV;
}


std::shared_ptr<Animation> ResourceManager::CreateAnimation(std::string filePath)
{
	auto it = m_AnimationMap.find(filePath);
	if (it != m_AnimationMap.end())
	{
		std::shared_ptr<Animation> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			m_AnimationMap.erase(it);
			// �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}
	
	std::filesystem::path path = ToWString(string(filePath));
	if (!std::filesystem::exists(path)) {
		LOG_MESSAGEA("Erorr file not found: %s", filePath.c_str());
		return nullptr;
	};

	GameTimer timer;
	timer.Tick();
	std::shared_ptr<Animation> pAnimation = std::make_shared<Animation>();
	pAnimation->Create(filePath);
	m_AnimationMap[filePath] = pAnimation;
	timer.Tick();
	LOG_MESSAGEA("Complete file: %s %f", filePath.c_str(), timer.DeltaTime());
	return pAnimation;
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

	std::filesystem::path path = ToWString(string(filePath));
	if (!std::filesystem::exists(path)) {
		LOG_MESSAGEA("Erorr file not found: %s", filePath.c_str());
		return nullptr;
	};
	
	GameTimer timer;
	timer.Tick();
	std::shared_ptr<SkeletalMeshSceneResource> pSceneResource = std::make_shared<SkeletalMeshSceneResource>();
	pSceneResource->Create(filePath.c_str());
	m_SkeletalMeshSceneMap[key] = pSceneResource;
	timer.Tick();
	LOG_MESSAGEA("Complete file: %s %f", filePath.c_str(), timer.DeltaTime());
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

	std::filesystem::path path = ToWString(string(filePath));
	if (!std::filesystem::exists(path)) {
		LOG_MESSAGEA("Erorr file not found: %s", filePath.c_str());
		return nullptr;
	};
	
	GameTimer timer;
	timer.Tick();
	std::shared_ptr<StaticMeshSceneResource> pSceneResource = std::make_shared<StaticMeshSceneResource>();
	pSceneResource->Create(filePath.c_str());
	m_StaticMeshSceneMap[key] = pSceneResource;
	timer.Tick();
	LOG_MESSAGEA("Complete file: %s %f", filePath.c_str(), timer.DeltaTime());
	return pSceneResource;
}

