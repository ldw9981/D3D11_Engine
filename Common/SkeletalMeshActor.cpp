#include "pch.h"
#include "SkeletalMeshActor.h"
#include "SkeletalMeshComponent.h"
#include "SkeletalMeshResource.h"
#include "BoxComponent.h"
#include "Actor.h"

SkeletalMeshActor::SkeletalMeshActor()
{
	m_pSkeletalMeshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent").get();
	SetRootComponent(m_pSkeletalMeshComponent);	

	m_pBoxComponent = CreateComponent<BoxComponent>("BoxComponent").get();
	m_pBoxComponent->SetParent(m_pSkeletalMeshComponent);
	m_pBoxComponent->SetCollisionType(CollisionType::Block);
}

void SkeletalMeshActor::OnBeginPlay()
{
	__super::OnBeginPlay();

	Math::Vector3 Center,Extents;
	float MaxExtent = 0.0f;
	m_pSkeletalMeshComponent->m_SceneResource->GetAABB(Center, Extents);
	MaxExtent = max(MaxExtent,Extents.x);
	MaxExtent = max(MaxExtent,Extents.y);
	MaxExtent = max(MaxExtent,Extents.z);

	m_pBoxComponent->m_Geomety.Center = Center;
	m_pBoxComponent->m_Extents = Vector3(MaxExtent,MaxExtent,MaxExtent);
}

