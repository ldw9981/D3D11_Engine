#include "pch.h"
#include "SkeletalMeshComponent.h"
#include "SceneComponent.h"
#include "Helper.h"
#include "ResourceManager.h"
#include "TimeSystem.h"
#include "SkeletonResource.h"
#include "SkeletalMeshResource.h"
#include "SkeletalMeshInstance.h"
#include "AnimationResource.h"
#include "D3DRenderManager.h"
#include "GameApp.h"
#include "TimeSystem.h"


SkeletalMeshComponent::SkeletalMeshComponent()
{
	D3DRenderManager::Instance->AddSkeletalMeshComponent(this);
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	D3DRenderManager::Instance->RemoveSkeletalMeshComponent(this);
}

void SkeletalMeshComponent::CreateMeshInstance(std::shared_ptr<SkeletalMeshSceneResource> val)
{
	m_SceneResource = val;

	// 리소스로 인스턴스 처리한다.
	CreateHierachy(&m_SceneResource->m_Skeleton);	//계층구조 생성	

	m_MeshInstances.resize(m_SceneResource->m_SkeletalMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_SkeletalMeshResources.size(); i++)
	{
		SkeletalMeshResource* meshResource = &m_SceneResource->m_SkeletalMeshResources[i];
		Material* material = &m_SceneResource->m_Materials[i];
		m_MeshInstances[i].Create(meshResource,&m_SceneResource->m_Skeleton,&m_RootBone,material);
	}
	UpdateBoneAnimationReference(0);	// 각 노드의 애니메이션 정보참조 연결	

	m_SceneResource->GetBoundingBox(m_BoundingBox.Center, m_BoundingBox.Extents);	
}

std::shared_ptr<SkeletalMeshSceneResource> SkeletalMeshComponent::GetSceneResource() const
{
	return m_SceneResource;
}

bool SkeletalMeshComponent::ReadSceneResourceFromFBX(std::string filePath)
{	
	// 리소스 매니저에서 가져온다.
	auto resource = ResourceManager::Instance->CreateSkeletalMeshSceneResource(filePath);	
	if (!resource) {
		return false;
	}
	m_SceneFilePath = filePath;
	CreateMeshInstance(resource);
	return true;
}



bool SkeletalMeshComponent::AddSceneAnimationFromFBX(std::string filePath)
{
	assert(m_SceneResource);
	auto animation = ResourceManager::Instance->CreateAnimationResource(filePath);
	if (!animation) {
		return false;
	}

	m_AnimationFilePath.push_back(filePath);
	m_SceneResource->m_Animations.push_back(animation);
	return true;
}

Material* SkeletalMeshComponent::GetMaterial(UINT index)
{
	assert(index < m_SceneResource->m_Materials.size());
	return &m_SceneResource->m_Materials[index];
}
void SkeletalMeshComponent::Update(float deltaTime)
{
	__super::Update(deltaTime);

	if (!m_SceneResource->m_Animations.empty())
	{
		m_AnimationProressTime += deltaTime;
		m_AnimationProressTime = (float)fmod(m_AnimationProressTime, m_SceneResource->m_Animations[m_AnimationIndex]->Duration);

	}	

	float passedTime = GameApp::m_pInstance->m_Timer.TotalTime() - m_AnimationLastTime;
	if (m_bIsCulled)// && passedTime > 0.001f)  		// 144프레임기준으로  ??? 다른컴에서는 왜 오작동
	{
		m_RootBone.Update(deltaTime);
		m_AnimationLastTime = GameApp::m_pInstance->m_Timer.TotalTime();
	}
		
}

void SkeletalMeshComponent::UpdateBoneAnimationReference(UINT index)
{
	assert(index < m_SceneResource->m_Animations.size());
	auto animation = m_SceneResource->m_Animations[index];
	for (size_t i = 0; i < animation->NodeAnimations.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation->NodeAnimations[i];
		Bone* pBone = m_RootBone.FindBone(nodeAnimation.NodeName);
		assert(pBone != nullptr);
		pBone->m_pNodeAnimation = &animation->NodeAnimations[i];
	}
}


void SkeletalMeshComponent::PlayAnimation(UINT index)
{
	assert(index < m_SceneResource->m_Animations.size());
	m_AnimationIndex = index;
	m_AnimationProressTime = 0.0f;
	UpdateBoneAnimationReference(index);
}

void SkeletalMeshComponent::CreateHierachy(SkeletonResource* skeleton)
{
	UINT count = skeleton->GetBoneCount();

	BoneInfo* pRootBone = skeleton->GetBone(0);
	m_RootBone.m_Name = pRootBone->Name;
	m_RootBone.m_Children.reserve(pRootBone->NumChildren);
	m_RootBone.SetParent(this);

	// 0번 루트는 컨테이너이므로 현재 Node와 같다 그러므로 1번부터 시작한다.
	for (UINT i = 1; i < count; i++)
	{
		BoneInfo* pBoneInfo = skeleton->GetBone(i);
		assert(pBoneInfo != nullptr);
		assert(pBoneInfo->ParentBoneIndex != -1);

		Bone* pParentBone = m_RootBone.FindBone(skeleton->GetBoneName(pBoneInfo->ParentBoneIndex));
		assert(pParentBone != nullptr);

		auto& ChildBone = pParentBone->CreateChild();
		ChildBone.m_Name = pBoneInfo->Name;
		ChildBone.m_Local = pBoneInfo->RelativeTransform;
		ChildBone.m_Children.reserve(pBoneInfo->NumChildren);
		ChildBone.m_pParent = pParentBone;
		ChildBone.m_pAnimationTime = &m_AnimationProressTime;
	}
}

void SkeletalMeshComponent::OnBeginPlay()
{
	__super::OnBeginPlay();
}

void SkeletalMeshComponent::OnEndPlay()
{
	__super::OnEndPlay();
}
