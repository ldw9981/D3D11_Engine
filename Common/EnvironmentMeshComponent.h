#pragma once
#include "RenderComponent.h"

class StaticMeshSceneResource;
class TextureResource;
class EnvironmentMeshComponent :
    public RenderComponent
{
public:
	EnvironmentMeshComponent();
	~EnvironmentMeshComponent();

	std::shared_ptr<StaticMeshSceneResource> m_SceneResource;
	std::shared_ptr<TextureResource> m_CubeTextureResource;

	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::string m_CubeTexturePath; // BeginPlay에서 로딩

	bool ReadEnvironmentMeshFromFBX(std::string filePath);
	bool ReadCubeTextureFromDDS(std::wstring filePath);
};

