#include "pch.h"
#include "SkeletalMeshActor.h"
#include "SkeletalMeshComponent.h"
#include "Actor.h"

SkeletalMeshActor::SkeletalMeshActor()
{
	m_pSkeletalMeshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(m_pSkeletalMeshComponent);
}
