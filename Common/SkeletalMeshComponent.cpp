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


SkeletalMeshComponent::SkeletalMeshComponent()
{
	D3DRenderManager::Instance->m_SkeletalMeshComponents.push_back(this);
	m_iterator = --D3DRenderManager::Instance->m_SkeletalMeshComponents.end();
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	D3DRenderManager::Instance->m_SkeletalMeshComponents.erase(m_iterator);
}

void SkeletalMeshComponent::SetSceneResource(std::shared_ptr<SkeletalMeshSceneResource> val)
{
	m_SceneResource = val;

	// 리소스로 인스턴스 처리한다.
	CreateHierachy(&m_SceneResource->m_Skeleton);	//계층구조 생성	

	m_MeshInstances.resize(m_SceneResource->m_SkeletalMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_SkeletalMeshResources.size(); i++)
	{
		m_MeshInstances[i].Create(&m_SceneResource->m_SkeletalMeshResources[i], // mesh resource
			&m_SceneResource->m_Skeleton,	 // skeleton resource
			&m_RootBone,	// root node
			m_SceneResource->GetMeshMaterial(i));		//material resource 
	}
	UpdateBoneAnimationReference(0);	// 각 노드의 애니메이션 정보참조 연결	

	// 캐릭터가 중점에 있다는 가정의 딱 맞는 AABB
	m_BoundingBox.Center = Math::Vector3(m_SceneResource->m_AABBmin + m_SceneResource->m_AABBmax)*0.5;	// Calculate extent
	m_BoundingBox.Extents = Math::Vector3(m_SceneResource->m_AABBmax - m_SceneResource->m_AABBmin)*0.5;	// Calculate extent
	

	// 팔벌릴때를 대비해서 z축과 x축중 큰값으로 AABB를 만든다.
	float max = std::fmax(m_BoundingBox.Extents.z, m_BoundingBox.Extents.x);
	max = std::fmax(max, m_BoundingBox.Extents.y);
	m_BoundingBox.Extents.z = max;
	m_BoundingBox.Extents.x = max;	
	m_BoundingBox.Extents.y = max;
	
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
	SetSceneResource(resource);
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

	if(m_bIsCulled)
		m_RootBone.Update(deltaTime);
}

void SkeletalMeshComponent::UpdateBoneAnimationReference(UINT index)
{
	assert(index < m_SceneResource->m_Animations.size());
	auto animation = m_SceneResource->m_Animations[index];
	for (size_t i = 0; i < animation->NodeAnimations.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation->NodeAnimations[i];
		Bone* pNode = m_RootBone.FindNode(nodeAnimation.NodeName);
		assert(pNode != nullptr);
		pNode->m_pNodeAnimation = &animation->NodeAnimations[i];
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
		BoneInfo* pBone = skeleton->GetBone(i);
		assert(pBone != nullptr);
		assert(pBone->ParentBoneIndex != -1);

		Bone* pParentNode = m_RootBone.FindNode(skeleton->GetBoneName(pBone->ParentBoneIndex));
		assert(pParentNode != nullptr);

		auto& node = pParentNode->CreateChild();
		node.m_Name = pBone->Name;
		node.m_Local = pBone->RelativeTransform;
		node.m_Children.reserve(pBone->NumChildren);
		node.m_pParent = pParentNode;
		node.m_pAnimationTime = &m_AnimationProressTime;
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

void SkeletalMeshComponent::CalculateBoundingBox()
{
	m_BoundingBox.Center = m_World.Translation();
	m_BoundingBox.Center.y = m_BoundingBox.Center.y + m_BoundingBox.Extents.y;
}
