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


	std::map<std::wstring , Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_TextureMap;	// Texture	

	std::map<std::string, std::weak_ptr<SkeletalMeshResource>> m_SkeletalMeshMap;	// SkeletalMesh

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTexture(std::wstring filePath);
	std::shared_ptr<SceneResource> CreateSceneResource(std::string filePath);
};

