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



bool StaticMeshModel::ReadSceneResourceFromFBX(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath.c_str());
	GameTimer timer;
	timer.Tick();
	// 리소스 매니저에서 가져온다.
	m_SceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource(filePath);
	if (!m_SceneResource) {
		return false;
	}

	//CreateHierachy(&m_SceneResource->m_Skeleton);	//StaticMesh는 계층구조는 사용 안하기로 한다.

	// 인스턴스 생성
	m_MeshInstances.resize(m_SceneResource->m_StaticMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_StaticMeshResources.size(); i++)
	{
		m_MeshInstances[i].Create(&m_SceneResource->m_StaticMeshResources[i], // mesh resource		
			this,	// root node
			m_SceneResource->GetMeshMaterial(i));		//material resource 
	}

	timer.Tick();
	LOG_MESSAGEA("Complete file: %s %f", filePath.c_str(), timer.DeltaTime());
	return true;
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