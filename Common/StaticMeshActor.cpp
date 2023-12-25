#include "pch.h"
#include "StaticMeshActor.h"

StaticMeshActor::StaticMeshActor()
{
	m_pStaticMeshComponent = CreateComponent<StaticMeshComponent>("StaticMeshComponent").get();
	SetRootComponent(m_pStaticMeshComponent);
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

}
