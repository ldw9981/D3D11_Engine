#include "pch.h"
#include "StaticMeshComponent.h"
#include "StaticMeshResource.h"
#include "ResourceManager.h"
#include "TimeSystem.h"
#include "Helper.h"


StaticMeshComponent::StaticMeshComponent()
{
	D3DRenderManager::Instance->m_StaticMeshComponents.push_back(this);
	m_iterator = --D3DRenderManager::Instance->m_StaticMeshComponents.end();
}

StaticMeshComponent::~StaticMeshComponent()
{
	D3DRenderManager::Instance->m_StaticMeshComponents.erase(m_iterator);
}

void StaticMeshComponent::Update(float DeltaTime)
{
	__super::Update(DeltaTime);
}

bool StaticMeshComponent::ReadSceneResourceFromFBX(std::string filePath)
{
	// FBX 파일 읽기
	std::shared_ptr<StaticMeshSceneResource> sceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource(filePath);
	if (sceneResource == nullptr)
	{
		return false;
	}
	SetSceneResource(sceneResource);
	return true;
}

void StaticMeshComponent::SetSceneResource(std::shared_ptr<StaticMeshSceneResource> val)
{
	assert(val);
	m_SceneResource = val;
	// 인스턴스 생성
	m_MeshInstances.resize(m_SceneResource->m_StaticMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_StaticMeshResources.size(); i++)
	{
		StaticMeshResource* meshResource = &m_SceneResource->m_StaticMeshResources[i];
		Material* material = &m_SceneResource->m_Materials[i];
		m_MeshInstances[i].Create(meshResource,&m_World,material);		//material resource 
	}

	m_BoundingBox.Center = Math::Vector3(m_SceneResource->m_AABBmin + m_SceneResource->m_AABBmax) * 0.5f;	// Calculate extent
	m_BoundingBox.Extents = Math::Vector3(m_SceneResource->m_AABBmax - m_SceneResource->m_AABBmin) * 0.5f;	// Calculate extent
}



Material* StaticMeshComponent::GetMaterial(UINT index)
{
	assert(index < m_SceneResource->m_Materials.size());
	return &m_SceneResource->m_Materials[index];
}

void StaticMeshComponent::OnBeginPlay()
{
	if(!m_SceneFilePath.empty())
		ReadSceneResourceFromFBX(m_SceneFilePath);
}

void StaticMeshComponent::OnEndPlay()
{
}
