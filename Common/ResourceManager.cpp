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

std::shared_ptr<Animation> ResourceManager::CreateAnimation(std::string key,const aiAnimation* pAiAnimation)
{
	// Ű�� �̹� ������� �����Ͱ� �ִ��� ã�´�.
	auto it = m_AnimationMap.find(key);
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
	std::shared_ptr<Animation> pAnimation = std::make_shared<Animation>();
	pAnimation->Create(pAiAnimation);
	m_AnimationMap[key] = pAnimation;
	return pAnimation;
}

ResourceManager::ResourceManager()
{
	Instance = this;
}

ResourceManager::~ResourceManager()
{
	
}



std::shared_ptr<Material> ResourceManager::CreateMaterial(std::string key,const aiMaterial* pAiMaterial)
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

std::shared_ptr<Skeleton> ResourceManager::CreateSkeleton(std::string key, const aiScene* pAiScene)
{
	// Ű�� �̹� ������� ���͸����� �ִ��� ã�´�.
	auto it = m_SkeletonMap.find(key);
	if (it != m_SkeletonMap.end())
	{
		std::shared_ptr<Skeleton> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			m_SkeletonMap.erase(it);
			// �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}
	std::shared_ptr<Skeleton> pSkeleton = std::make_shared<Skeleton>();
	pSkeleton->Create(pAiScene);
	m_SkeletonMap[key] = pSkeleton;
	return pSkeleton;
}

