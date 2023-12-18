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

std::shared_ptr<MaterialTexture> ResourceManager::CreateMaterialTexture(std::wstring filePath)
{	
	auto it = m_MaterialTextureMap.find(filePath);
	if (it != m_MaterialTextureMap.end())
	{
		std::shared_ptr<MaterialTexture> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이라 메모리가 아직 살아있다면 resourcePtr를 리턴한다.
		{
			return resourcePtr;
		}
		else  //UseCount가 0이라면 메모리가 이미 해제되었으므로 맵에서 제거한다.
		{
			m_MaterialTextureMap.erase(it);
			// 리턴하지 않고 아래에서 새로 만들어서 리턴한다.
		}
	}

	std::shared_ptr<MaterialTexture> pTextureRV = std::make_shared<MaterialTexture>();
	pTextureRV->Create(filePath);
	m_MaterialTextureMap[filePath] = pTextureRV;	
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
	// 키로 이미 만들어진 머터리얼이 있는지 찾는다.
	std::string key = filePath;
	auto it = m_SceneMap.find(key);
	if (it != m_SceneMap.end())
	{
		std::shared_ptr<SceneResource> resourcePtr = it->second.lock();
		if (resourcePtr)  //UseCount가 1이상이라 메모리가 아직 살아있다면 resourcePtr를 리턴한다.
		{
			return resourcePtr;
		}
		else  //UseCount가 0이라면 메모리가 이미 해제되었으므로 맵에서 제거한다.
		{
			m_SceneMap.erase(it);
			// 리턴하지 않고 아래에서 새로 만들어서 리턴한다.
		}
	}

	std::shared_ptr<SceneResource> pSceneResource = std::make_shared<SceneResource>();
	pSceneResource->Create(filePath.c_str());
	m_SceneMap[key] = pSceneResource;
	return pSceneResource;
}

