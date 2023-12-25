#include "pch.h"
#include "SkeletalMeshActor.h"
#include "SkeletalMeshComponent.h"
#include "Actor.h"

SkeletalMeshActor::SkeletalMeshActor()
{
	m_pSkeletalMeshComponent = CreateComponent<SkeletalMeshComponent>("SkeletalMeshComponent").get();
	SetRootComponent(m_pSkeletalMeshComponent);	
}

void SkeletalMeshActor::OnBeginPlay()
{
	__super::OnBeginPlay();
	if (!m_SceneFilePath.empty())
	{
		m_pSkeletalMeshComponent->ReadSceneResourceFromFBX(m_SceneFilePath);
	}
}

