#pragma once
#include "Singleton.h"
#include <memory>
#include "Material.h"

class ResourceManager : public Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();

	
	// Texture

	// StaticMesh

	// SkeletalMesh

	// Animation

	// Material
	std::map<std::wstring,std::shared_ptr<Material>> m_Materials;

	std::shared_ptr<Material> GetMaterialFromFBX(std::wstring FilePath);

	void Initialize();
	void Uninitialize();
};

