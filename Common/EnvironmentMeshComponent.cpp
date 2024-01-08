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
	m_MeshInstance.Create(&m_SceneResource->m_StaticMeshResources[0], &m_World, nullptr);
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

bool EnvironmentMeshComponent::ReadIrradianceTextureFromDDS(std::wstring filePath)
{
	m_IrradianceTextureResource = ResourceManager::Instance->CreateTextureResource(filePath);
	assert(m_IrradianceTextureResource != nullptr);
	if (m_IrradianceTextureResource == nullptr)
	{
		return false;
	}
	return true;
}

bool EnvironmentMeshComponent::ReadRadianceTextureFromDDS(std::wstring filePath)
{
	m_RadianceTextureResource = ResourceManager::Instance->CreateTextureResource(filePath);
	assert(m_RadianceTextureResource != nullptr);
	if (m_RadianceTextureResource == nullptr)
	{
		return false;
	}
	return true;
}
