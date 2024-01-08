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
	std::shared_ptr<TextureResource>	m_EnvironmentTextureResource;
	std::shared_ptr<TextureResource>	m_IBLDiffuseTextureResource;
	std::shared_ptr<TextureResource>	m_IBLSpecularTextureResource;
	std::shared_ptr<TextureResource>	m_IBLBRDFTextureResource;
	StaticMeshInstance					m_MeshInstance;

	std::string m_SceneFilePath; // BeginPlay���� �ε�
	std::wstring m_EnvironmentTexturePath; // BeginPlay���� �ε�
	std::wstring m_IBLSpecularTexturePath; // BeginPlay���� �ε�
	std::wstring m_IBLDiffuseTexturePath; // BeginPlay���� �ε�
	std::wstring m_IBLBRDFTexturePath; // BeginPlay���� �ε�

	bool ReadEnvironmentMeshFromFBX(std::string filePath);
	bool ReadEnvironmentTextureFromDDS(std::wstring filePath);
	bool ReadIBLDiffuseTextureFromDDS(std::wstring filePath);
	bool ReadIBLSpecularTextureFromDDS(std::wstring filePath);
	bool ReadIBLBRDFTextureFromDDS(std::wstring filePath);
};

