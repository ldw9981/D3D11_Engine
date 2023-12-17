#include "pch.h"
#include "ResourceManager.h"
#include "D3DRenderManager.h"
#include "Helper.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>


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


std::shared_ptr<SceneResource> ResourceManager::CreateSceneResource(std::string filePath)
{
	// Ű�� �̹� ������� ���͸����� �ִ��� ã�´�.
	std::string key = filePath;
	auto it = m_SceneMap.find(key);
	if (it != m_SceneMap.end())
	{
		std::shared_ptr<SceneResource> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount�� 1�̻��̶� �޸𸮰� ���� ����ִٸ� resourcePtr�� �����Ѵ�.
		{
			return resourcePtr;
		}
		else  //UseCount�� 0�̶�� �޸𸮰� �̹� �����Ǿ����Ƿ� �ʿ��� �����Ѵ�.
		{
			m_SceneMap.erase(it);
			// �������� �ʰ� �Ʒ����� ���� ���� �����Ѵ�.
		}
	}

	std::shared_ptr<SceneResource> pSceneResource = std::make_shared<SceneResource>();
	pSceneResource->Create(filePath.c_str());
	m_SceneMap[key] = pSceneResource;
	return pSceneResource;
}

