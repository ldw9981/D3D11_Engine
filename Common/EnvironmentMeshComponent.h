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
	std::shared_ptr<TextureResource>	m_IrradianceTextureResource;
	std::shared_ptr<TextureResource>	m_RadianceTextureResource;
	StaticMeshInstance					m_MeshInstance;

	std::string m_SceneFilePath; // BeginPlay���� �ε�
	std::wstring m_CubeTexturePath; // BeginPlay���� �ε�
	std::wstring m_IrradianceTexturePath; // BeginPlay���� �ε�
	std::wstring m_RadianceTexturePath; // BeginPlay���� �ε�

	bool ReadEnvironmentMeshFromFBX(std::string filePath);
	bool ReadCubeTextureFromDDS(std::wstring filePath);
	bool ReadIrradianceTextureFromDDS(std::wstring filePath);
	bool ReadRadianceTextureFromDDS(std::wstring filePath);
};

