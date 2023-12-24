#pragma once
#include "SceneComponent.h"
#include "Bone.h"
#include "SkeletalMeshInstance.h"


class Actor;
class SkeletalMeshComponent :
    public SceneComponent
{
public:
    SkeletalMeshComponent() {}
    SkeletalMeshComponent(Actor* pOwner, const std::string& Name);
	virtual ~SkeletalMeshComponent();

};

//  이후에 Component로 변경될 예정입니다.
class Material;
class SkeletalMeshSceneResource;
class SkeletalMeshInstance;
class SkeletalMeshModel : public Bone
{
public:
	SkeletalMeshModel() {}
	~SkeletalMeshModel() {}

	std::shared_ptr<SkeletalMeshSceneResource>  m_SceneResource;

	void SetSceneResource(std::shared_ptr<SkeletalMeshSceneResource> val);
	std::vector<SkeletalMeshInstance> m_MeshInstances;

	float m_AnimationProressTime = 0.0f;
	UINT  m_AnimationIndex = 0;

	bool ReadSceneResourceFromFBX(std::string filePath);


	Material* GetMaterial(UINT index);

	void Update(float deltaTime);

	void UpdateNodeAnimationReference(UINT index);
	void SetWorldTransform(const Math::Matrix& transform);
	void PlayAnimation(UINT index);
	void CreateHierachy(SkeletonResource* skeleton);
};