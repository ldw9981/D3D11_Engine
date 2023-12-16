#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Node.h"
#include "Skeleton.h"


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


class StaticMeshModel: public Node
{
public:
	StaticMeshModel() {}
	~StaticMeshModel() {}
		
	std::vector<MeshInstance> m_Meshes;	
	std::vector<shared_ptr<Material>> m_Materials;
	std::vector<shared_ptr<Animation>> m_Animations;

	shared_ptr<Skeleton> m_pSkeleton = nullptr;
	
	float m_AnimationProressTime = 0.0f;
    

	bool ReadFile(ID3D11Device* device,const char* filePath);
		
	Material* GetMaterial(UINT index);

	void Update(float deltaTime);	

	void UpdateNodeAnimationReference(UINT index);
	void SetWorldTransform(const Math::Matrix& transform);
};


class SkeletalMeshModel : public Node
{
public:
	SkeletalMeshModel() {}
	~SkeletalMeshModel() {}

	std::vector<MeshInstance> m_Meshes;
	std::vector<shared_ptr<Material>> m_Materials;
	std::vector<shared_ptr<Animation>> m_Animations;

	shared_ptr<Skeleton> m_pSkeleton = nullptr;

	float m_AnimationProressTime = 0.0f;


	bool ReadFile(ID3D11Device* device, const char* filePath);

	Material* GetMaterial(UINT index);

	void Update(float deltaTime);

	void UpdateNodeAnimationReference(UINT index);
	void SetWorldTransform(const Math::Matrix& transform);
};