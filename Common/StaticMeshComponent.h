#pragma once
#include "SceneComponent.h"
#include "StaticMeshInstance.h"
#include "Bone.h"

class Actor;
class Material;
class StaticMeshSceneResource;
// ���Ŀ� Component�� ����� �����Դϴ�.
class StaticMeshComponent :
    public SceneComponent
{
public:
	StaticMeshComponent() {}
	StaticMeshComponent(Actor* pOwner, const std::string& Name);
	~StaticMeshComponent();

	std::string m_SceneFilePath; // BeginPlay���� �ε�
	std::shared_ptr<StaticMeshSceneResource>  m_SceneResource;
	std::vector<StaticMeshInstance> m_MeshInstances;

	virtual void Update(float DeltaTime) override;
	
	bool ReadSceneResourceFromFBX(std::string filePath);
	void SetSceneResource(std::shared_ptr<StaticMeshSceneResource> val);
	
	Material* GetMaterial(UINT index);


	virtual void OnBeginPlay();
	virtual void OnEndPlay();
};


