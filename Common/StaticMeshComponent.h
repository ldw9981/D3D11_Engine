#pragma once
#include "SceneComponent.h"
#include "StaticMeshInstance.h"
#include "Bone.h"

class Actor;
class Material;
class StaticMeshSceneResource;
// 이후에 Component로 변경될 예정입니다.
class StaticMeshComponent :
    public SceneComponent
{
public:
	StaticMeshComponent() {}
	StaticMeshComponent(Actor* pOwner, const std::string& Name);
	~StaticMeshComponent();


	std::shared_ptr<StaticMeshSceneResource>  m_SceneResource;
	std::vector<StaticMeshInstance> m_MeshInstances;

	virtual void Update(float DeltaTime) override;
	
	bool LoadFBX(const std::string& FileName);
	void SetSceneResource(std::shared_ptr<StaticMeshSceneResource> val);
	
	Material* GetMaterial(UINT index);
};


