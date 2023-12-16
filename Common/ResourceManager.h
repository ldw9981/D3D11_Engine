#pragma once
#include "Singleton.h"
#include "Material.h"
#include "Animation.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* Instance;
	
	std::map<std::wstring , Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_TextureMap;	// Texture	
	std::map<std::string, std::weak_ptr<Animation>> m_AnimationMap;	// Animation	
	std::map<std::string, std::weak_ptr<Material>> m_MaterialMap;	// Material
	// StaticMesh

	// SkeletalMesh

	// Skeleton
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTexture(std::wstring filePath);
	std::shared_ptr<Animation> CreateAnimation(std::string key, const aiAnimation* pAiAnimation);
	std::shared_ptr<Material> CreateMaterial(std::string key, const aiMaterial* pAiMaterial);
};

