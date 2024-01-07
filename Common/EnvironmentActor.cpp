#include "pch.h"
#include "EnvironmentActor.h"
#include "EnvironmentMeshComponent.h"
#include "D3DRenderManager.h"	
EnvironmentActor::EnvironmentActor()
{
	auto pComponent = CreateComponent<EnvironmentMeshComponent>("EnvironmentMeshComponent");
	m_pEnvironmentMeshComponent = pComponent.get();
	SetRootComponent(m_pEnvironmentMeshComponent);	

	// ��ϵȰ��� ������ ����Ѵ�
	auto pMesh = D3DRenderManager::Instance->GetEnvironmentMeshComponent();
	if(pMesh.expired())
		D3DRenderManager::Instance->SetEnvironmentMeshComponent(pComponent);
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
