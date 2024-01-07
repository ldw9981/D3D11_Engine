#include "pch.h"
#include "SkeletalMeshActor.h"
#include "SkeletalMeshComponent.h"
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
	m_pBoxComponent->m_Geomety = m_pSkeletalMeshComponent->m_BoundingBox;
}

