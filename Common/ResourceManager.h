#pragma once
#include "Singleton.h"


class MaterialTexture;
class StaticMeshSceneResource;
class SkeletalMeshSceneResource;
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* Instance;
	
	std::map<std::string, std::weak_ptr<StaticMeshSceneResource>> m_StaticMeshSceneMap;
	std::map<std::string, std::weak_ptr<SkeletalMeshSceneResource>> m_SkeletalMeshSceneMap;
	std::map<std::wstring ,std::weak_ptr<MaterialTexture>> m_MaterialTextureMap;	// Texture	


	
	std::shared_ptr<StaticMeshSceneResource> CreateStaticMeshSceneResource(std::string filePath);
	std::shared_ptr<SkeletalMeshSceneResource> CreateSkeletalMeshSceneResource(std::string filePath);
	std::shared_ptr<MaterialTexture> CreateMaterialTexture(std::wstring filePath);	
};

