#pragma once
#include "SceneComponent.h"
#include "StaticMeshInstance.h"
#include "Node.h"

class Actor;
class Material;
class StaticMeshSceneResource;
class StaticMeshComponent :
    public SceneComponent
{
public:
	StaticMeshComponent(Actor* pOwner, const std::string& Name);
	~StaticMeshComponent();

	virtual void Update(float DeltaTime) override;

	bool LoadFBX(const std::string& FileName);
};

// 이후에 Component로 변경될 예정입니다.
class StaticMeshSceneResource;
class StaticMeshModel : public Node
{
public:
	StaticMeshModel() {}
	~StaticMeshModel() {}

	std::shared_ptr<StaticMeshSceneResource>  m_SceneResource;
	std::vector<StaticMeshInstance> m_MeshInstances;

	void SetSceneResource(std::shared_ptr<StaticMeshSceneResource> val);
	bool ReadSceneResourceFromFBX(std::string filePath);
	Material* GetMaterial(UINT index);


	void Update(float deltaTime);
	void SetWorldTransform(const Math::Matrix& transform);
};