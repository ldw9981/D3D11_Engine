#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Node.h"
#include "SkeletonInfo.h"


struct MetaData
{
	std::string Name;
	int Type = -1;
	std::string metadataStr;
	float	metadataFloat;
	int		metadataInt;
	float	metadataVector[3];

	void SetData(const aiMetadataEntry& entry);
};

// �ϳ��� StaticMesh FBX�� �ǹ��ϴ� ����������, ���� �޽��� ���� �� �ִ�.
class StaticMeshSceneResource
{
public:
	StaticMeshSceneResource() {}
	~StaticMeshSceneResource() {}

	Math::Vector3 m_AABBmin;
	Math::Vector3 m_AABBmax;
	std::vector<StaticMeshResource> m_StaticMeshResources;
	std::vector<Material> m_Materials;

	bool Create(std::string filePath);
	Material* GetMeshMaterial(UINT index);
};


class SkeletalMeshSceneResource
{
public:
	SkeletalMeshSceneResource() {}
	~SkeletalMeshSceneResource() {}

	Math::Vector3 m_AABBmin;
	Math::Vector3 m_AABBmax;
	std::vector<SkeletalMeshResource> m_SkeletalMeshResources;
	std::vector<Material> m_Materials;
	SkeletonInfo m_Skeleton;
		
	std::vector<Animation> m_Animations;

	bool Create(std::string filePath);
	bool AddAnimation(std::string filePath);
	Material* GetMeshMaterial(UINT index);	
};


// ���Ŀ� Component�� ����� �����Դϴ�.
class StaticMeshModel: public Node
{
public:
	StaticMeshModel() {}
	~StaticMeshModel() {}
		
	shared_ptr<StaticMeshSceneResource>  m_SceneResource;
	std::vector<StaticMeshInstance> m_MeshInstances;
	    

	bool ReadSceneResourceFromFBX(std::string filePath);
	Material* GetMaterial(UINT index);


	void Update(float deltaTime);	
	void SetWorldTransform(const Math::Matrix& transform);
};

//  ���Ŀ� Component�� ����� �����Դϴ�.
class SkeletalMeshModel : public Node
{
public:
	SkeletalMeshModel() {}
	~SkeletalMeshModel() {}

	shared_ptr<SkeletalMeshSceneResource>  m_SceneResource;
	std::vector<SkeletalMeshInstance> m_MeshInstances;

	float m_AnimationProressTime = 0.0f;
	UINT  m_AnimationIndex = 0;

	bool ReadSceneResourceFromFBX(std::string filePath);
	bool ReadAnimationOnlyFromFBX(std::string filePath);

	Material* GetMaterial(UINT index);

	void Update(float deltaTime);

	void UpdateNodeAnimationReference(UINT index);
	void SetWorldTransform(const Math::Matrix& transform);
	void PlayAnimation(UINT index);
	void CreateHierachy(SkeletonInfo* skeleton);
};