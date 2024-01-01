#include "pch.h"
#include "StaticMeshActor.h"
#include "BoxComponent.h"
#include "StaticMeshComponent.h"
#include "StaticMeshResource.h"


StaticMeshActor::StaticMeshActor()
{
	m_pStaticMeshComponent = CreateComponent<StaticMeshComponent>("StaticMeshComponent").get();
	SetRootComponent(m_pStaticMeshComponent);
	m_pBoxComponent = CreateComponent<BoxComponent>("BoxComponent").get();	
	m_pBoxComponent->SetParent(m_pStaticMeshComponent);
	m_pBoxComponent->SetCollisionType(CollisionType::Block);
}

StaticMeshActor::~StaticMeshActor()
{

}

void StaticMeshActor::OnBeginPlay()
{
	__super::OnBeginPlay();
	if (!m_SceneFilePath.empty())
	{
		m_pStaticMeshComponent->ReadSceneResourceFromFBX(m_SceneFilePath);
	}

	m_pBoxComponent->m_Geomety = m_pStaticMeshComponent->m_BoundingBox;	
}
