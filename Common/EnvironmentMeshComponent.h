#pragma once
#include "RenderComponent.h"
#include "StaticMeshInstance.h"

class StaticMeshSceneResource;
class TextureResource;
class EnvironmentMeshComponent :
    public RenderComponent
{
public:
	EnvironmentMeshComponent();
	~EnvironmentMeshComponent();

	std::shared_ptr<StaticMeshSceneResource> m_SceneResource;
	std::shared_ptr<TextureResource>	m_CubeTextureResource;
	StaticMeshInstance					m_MeshInstance;

	std::string m_SceneFilePath; // BeginPlay���� �ε�
	std::string m_CubeTexturePath; // BeginPlay���� �ε�

	bool ReadEnvironmentMeshFromFBX(std::string filePath);
	bool ReadCubeTextureFromDDS(std::wstring filePath);
};

