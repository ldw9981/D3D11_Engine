#pragma once
#include "Singleton.h"
#include <memory>
#include "Material.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();

	// StaticMesh

	// SkeletalMesh

	// Skeleton

	// Animation

	// Material
	std::map<std::string,std::weak_ptr<Material>> m_MaterialMap;

	std::shared_ptr<Material> CreateMaterial(std::string key,aiMaterial* pAiMaterial);

	void Initialize();
	void Uninitialize();
};

