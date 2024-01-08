#include "pch.h"
#include "EnvironmentActor.h"
#include "EnvironmentMeshComponent.h"
#include "D3DRenderManager.h"	
EnvironmentActor::EnvironmentActor()
{
	auto pComponent = CreateComponent<EnvironmentMeshComponent>("EnvironmentMeshComponent");
	m_pEnvironmentMeshComponent = pComponent.get();
	SetRootComponent(m_pEnvironmentMeshComponent);	
}

EnvironmentActor::~EnvironmentActor()
{

}

void EnvironmentActor::OnBeginPlay()
{

}

void EnvironmentActor::OnEndPlay()
{

}
