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

	// FBX���� ��η� ����ƽ �޽�,���͸��� ���� ������
	std::shared_ptr<StaticMeshSceneResource> CreateStaticMeshSceneResource(std::string filePath);

	// FBX���� ��η� ���̷�Ż �޽�,���͸��� ����,�⺻ ���� �ִϸ��̼� ������
	std::shared_ptr<SkeletalMeshSceneResource> CreateSkeletalMeshSceneResource(std::string filePath);

	// ���� ���͸��� �������� ���� �ؽ�ó�� ����Ҽ� �����Ƿ� ���� ��Ͽ��� ���ϰ�� Ȯ���ؼ� ������
	std::shared_ptr<TextureResource> CreateTextureResource(std::wstring filePath);	

	// FBX���� ��η� �ִϸ��̼� ���� ������
	std::shared_ptr<AnimationResource> CreateAnimationResource(std::string filePath);
};

