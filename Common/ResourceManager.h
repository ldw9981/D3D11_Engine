#pragma once
#include "Singleton.h"
#include "Material.h"
#include "Animation.h"
#include "Skeleton.h"
#include "Mesh.h"
#include "Model.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* Instance;
	
	std::map<std::string, std::weak_ptr<SceneResource>> m_SceneMap;
	std::map<std::wstring ,std::weak_ptr<MaterialTexture>> m_MaterialTextureMap;	// Texture	


	std::shared_ptr<MaterialTexture> CreateMaterialTexture(std::wstring filePath);
	std::shared_ptr<SceneResource> CreateSceneResource(std::string filePath);
};

