#include "pch.h"
#include "StaticMeshComponent.h"
#include "StaticMeshResource.h"
#include "ResourceManager.h"
#include "TimeSystem.h"
#include "Helper.h"


StaticMeshComponent::StaticMeshComponent(Actor* pOwner, const std::string& Name)
	:SceneComponent(pOwner,Name)
{

}


StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::Update(float DeltaTime)
{
}

bool StaticMeshComponent::LoadFBX(const std::string& FileName)
{
	return true;
}



void StaticMeshModel::SetSceneResource(std::shared_ptr<StaticMeshSceneResource> val)
{
	assert(val);
	m_SceneResource = val;
	// 인스턴스 생성
	m_MeshInstances.resize(m_SceneResource->m_StaticMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_StaticMeshResources.size(); i++)
	{
		m_MeshInstances[i].Create(&m_SceneResource->m_StaticMeshResources[i], // mesh resource		
			this,	// root node
			m_SceneResource->GetMeshMaterial(i));		//material resource 
	}
}



Material* StaticMeshModel::GetMaterial(UINT index)
{
	assert(index < m_SceneResource->m_Materials.size());
	return &m_SceneResource->m_Materials[index];
}

void StaticMeshModel::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void StaticMeshModel::SetWorldTransform(const Math::Matrix& transform)
{
	m_Local = transform;
}