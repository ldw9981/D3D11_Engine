#include "pch.h"
#include "EnvironmentMeshComponent.h"
#include "ResourceManager.h"
#include "StaticMeshResource.h"


EnvironmentMeshComponent::EnvironmentMeshComponent()
{
	
}

EnvironmentMeshComponent::~EnvironmentMeshComponent()
{

}

bool EnvironmentMeshComponent::ReadEnvironmentMeshFromFBX(std::string filePath)
{
	// FBX 파일 읽기
	
	m_SceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource(filePath);
	assert(m_SceneResource != nullptr);
	if (m_SceneResource == nullptr)
	{
		return false;
	}
	return true;
}

bool EnvironmentMeshComponent::ReadCubeTextureFromDDS(std::wstring filePath)
{
	m_CubeTextureResource = ResourceManager::Instance->CreateTextureResource(filePath);
	assert(m_CubeTextureResource != nullptr);
	if (m_CubeTextureResource == nullptr)
	{
		return false;
	}
	return true;
}
