#pragma once
#include "RenderComponent.h"
#include "StaticMeshInstance.h"
#include "Bone.h"

class Actor;
class Material;
class StaticMeshSceneResource;
// 이후에 Component로 변경될 예정입니다.
class StaticMeshComponent :
    public RenderComponent
{
public:
	StaticMeshComponent();
	~StaticMeshComponent();

	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::shared_ptr<StaticMeshSceneResource>  m_SceneResource;
	std::vector<StaticMeshInstance> m_MeshInstances;
	
	std::list< StaticMeshComponent*>::iterator m_ItRenderManager;

	virtual void Update(float DeltaTime) override;
	
	bool ReadSceneResourceFromFBX(std::string filePath);
	void CreateMeshInstance(std::shared_ptr<StaticMeshSceneResource> val);
	
	Material* GetMaterial(UINT index);


	virtual void OnBeginPlay();
	virtual void OnEndPlay();
};


