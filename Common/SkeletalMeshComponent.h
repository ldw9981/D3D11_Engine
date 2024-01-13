#pragma once
#include "SceneComponent.h"
#include "Bone.h"
#include "SkeletalMeshInstance.h"
#include "RenderComponent.h"

class Actor;


//  이후에 Component로 변경될 예정입니다.
class Material;
class SkeletalMeshSceneResource;
class SkeletalMeshInstance;
class SkeletalMeshComponent : public RenderComponent
{
public:
	friend class D3DRenderManager;		// D3DRenderManager는 멤버에 제한없이 접근가능하게 한다.

	SkeletalMeshComponent();
	~SkeletalMeshComponent();
	
	void SetSceneFilePath(std::string val) { m_SceneFilePath = val; }
public:
	std::shared_ptr<SkeletalMeshSceneResource>  m_SceneResource;
protected:	
	std::vector<SkeletalMeshInstance> m_MeshInstances;
	float m_AnimationProressTime = 0.0f;
	UINT  m_AnimationIndex = 0;
	Bone m_RootBone;
	std::string m_SceneFilePath;				// BeginPlay에서 로딩	
	std::list<std::string> m_AnimationFilePath; // BeginPlay에서 로딩	

	std::list< SkeletalMeshComponent*>::iterator m_ItRenderManager;

public:
	void CreateMeshInstance(std::shared_ptr<SkeletalMeshSceneResource> val);
	std::shared_ptr<SkeletalMeshSceneResource> GetSceneResource() const;
	bool ReadSceneResourceFromFBX(std::string filePath);	
	bool AddSceneAnimationFromFBX(std::string filePath);
	Material* GetMaterial(UINT index);
	virtual void Update(float deltaTime) override;
	void UpdateBoneAnimationReference(UINT index);
	void PlayAnimation(UINT index);
	void CreateHierachy(SkeletonResource* skeleton);
	virtual void OnBeginPlay() override;
	virtual void OnEndPlay() override;
};