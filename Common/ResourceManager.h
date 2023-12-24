#pragma once

class MaterialTexture;
class StaticMeshSceneResource;
class SkeletalMeshSceneResource;
struct AnimationResource;
class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* Instance;
	
	std::map<std::string, std::weak_ptr<StaticMeshSceneResource>> m_StaticMeshSceneMap;
	std::map<std::string, std::weak_ptr<SkeletalMeshSceneResource>> m_SkeletalMeshSceneMap;
	std::map<std::wstring ,std::weak_ptr<MaterialTexture>> m_MaterialTextureMap;	// Texture	
	std::map<std::string, std::weak_ptr<AnimationResource>> m_AnimationMap;	// Animation

	
	std::shared_ptr<StaticMeshSceneResource> CreateStaticMeshSceneResource(std::string filePath);
	std::shared_ptr<SkeletalMeshSceneResource> CreateSkeletalMeshSceneResource(std::string filePath);
	std::shared_ptr<MaterialTexture> CreateMaterialTexture(std::wstring filePath);	
	std::shared_ptr<AnimationResource> CreateAnimationResource(std::string filePath);
};

