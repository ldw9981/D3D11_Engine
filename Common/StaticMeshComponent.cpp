#include "pch.h"
#include "StaticMeshComponent.h"
#include "StaticMeshResource.h"
#include "ResourceManager.h"
#include "TimeSystem.h"
#include "Helper.h"


StaticMeshComponent::StaticMeshComponent()
{
	ADD_PROPERTY(m_SceneFilePath);
	D3DRenderManager::Instance->AddStaticMeshComponent(this);
}

StaticMeshComponent::~StaticMeshComponent()
{
	D3DRenderManager::Instance->RemoveStaticMeshComponent(this);
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

	m_SceneFilePath = filePath;
	CreateMeshInstance(sceneResource);
	return true;
}

void StaticMeshComponent::CreateMeshInstance(std::shared_ptr<StaticMeshSceneResource> val)
{
	assert(val);
	m_SceneResource = val;
	// 인스턴스 생성
	m_MeshInstances.resize(m_SceneResource->m_StaticMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_StaticMeshResources.size(); i++)
	{
		StaticMeshResource* meshResource = &m_SceneResource->m_StaticMeshResources[i];
		UINT materialIndex = meshResource->m_MaterialIndex;
		assert(materialIndex < m_SceneResource->m_Materials.size());
		Material* material = &m_SceneResource->m_Materials[materialIndex];
		m_MeshInstances[i].Create(meshResource,&m_World,material);		//material resource 
	}		
	m_BoundingBox.Center =  Math::Vector3(m_SceneResource->m_BoundingBoxMin + m_SceneResource->m_BoundingBoxMax) * 0.5f;	// Calculate extent	
	m_BoundingBox.Extents = Math::Vector3(m_SceneResource->m_BoundingBoxMax - m_SceneResource->m_BoundingBoxMin) * 0.5f;	// Calculate extent
}



Material* StaticMeshComponent::GetMaterial(UINT index)
{
	assert(index < m_SceneResource->m_Materials.size());
	return &m_SceneResource->m_Materials[index];
}

void StaticMeshComponent::OnBeginPlay()
{
	__super::OnBeginPlay();
}

void StaticMeshComponent::OnEndPlay()
{
}

void StaticMeshComponent::OnEnterStringImGUI(std::string PropertyName, std::string PropertyData)
{
	if (PropertyName == "m_SceneFilePath")
	{		
		std::filesystem::path path = std::filesystem::current_path();
		//std::string lastpath = path.parent_path().string() + std::string("\\");

		D3DRenderManager::Instance->RequestFileOpenDialog(".fbx", m_OnResultOpenFileDialog);
	}
}


void StaticMeshComponent::OnResultOpenFileDialog(const imgui_addons::ImGuiFileBrowser& dialog)
{
	if (dialog.selected_path.empty() == false)
	{		
		std::filesystem::path relative = std::filesystem::relative(dialog.selected_path, std::filesystem::current_path());
		ReadSceneResourceFromFBX(relative.string());
	}
}
