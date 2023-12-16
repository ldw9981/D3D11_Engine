#pragma once
#include "Singleton.h"
#include "Material.h"


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* Instance;
	// Texture
	std::map<std::wstring , Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_TextureMap;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTexture(std::wstring filePath);

	// StaticMesh

	// SkeletalMesh

	// Skeleton

	// Animation

	// Material
	std::map<std::string,std::weak_ptr<Material>> m_MaterialMap;

	std::shared_ptr<Material> CreateMaterial(std::string key,aiMaterial* pAiMaterial);
};

