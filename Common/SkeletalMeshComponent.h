#pragma once
#include "SceneComponent.h"
#include "Bone.h"
#include "SkeletalMeshInstance.h"


class Actor;


//  ���Ŀ� Component�� ����� �����Դϴ�.
class Material;
class SkeletalMeshSceneResource;
class SkeletalMeshInstance;
class SkeletalMeshComponent : public SceneComponent
{
public:
	SkeletalMeshComponent() {}
	SkeletalMeshComponent(Actor* pOwner, const std::string& Name);
	~SkeletalMeshComponent() {}

	std::shared_ptr<SkeletalMeshSceneResource>  m_SceneResource;
	std::vector<SkeletalMeshInstance> m_MeshInstances;

	float m_AnimationProressTime = 0.0f;
	UINT  m_AnimationIndex = 0;

	Bone m_RootBone;

	void SetSceneResource(std::shared_ptr<SkeletalMeshSceneResource> val);
	bool ReadSceneResourceFromFBX(std::string filePath);
	Material* GetMaterial(UINT index);
	virtual void Update(float deltaTime);
	void UpdateNodeAnimationReference(UINT index);
	void PlayAnimation(UINT index);
	void CreateHierachy(SkeletonResource* skeleton);
};