#pragma once

class TextureResource;
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
	std::map<std::wstring ,std::weak_ptr<TextureResource>> m_MaterialTextureMap;	// Texture	
	std::map<std::string, std::weak_ptr<AnimationResource>> m_AnimationMap;	// Animation

	// FBX파일 경로로 스테틱 메시,머터리얼 정보 얻어오기
	std::shared_ptr<StaticMeshSceneResource> CreateStaticMeshSceneResource(std::string filePath);

	// FBX파일 경로로 스켈레탈 메시,머터리얼 정보,기본 포즈 애니메이션 얻어오기
	std::shared_ptr<SkeletalMeshSceneResource> CreateSkeletalMeshSceneResource(std::string filePath);

	// 같은 머터리얼 내에서도 같은 텍스처를 사용할수 있으므로 공유 목록에서 파일경로 확인해서 얻어오기
	std::shared_ptr<TextureResource> CreateTextureResource(std::wstring filePath);	

	// FBX파일 경로로 애니메이션 정보 얻어오기
	std::shared_ptr<AnimationResource> CreateAnimationResource(std::string filePath);
};

