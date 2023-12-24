#pragma once
#include "SkeletonResource.h"
#include "StaticMeshResource.h"
#include "Animation.h"
#include "Material.h"

struct BoneWeightVertex
{
	Math::Vector3 Position;		// 정점 위치 정보.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
	Math::Vector3 BiTangent;

	// 영향받는 본수는 최대4개로 제한한다.
	int BlendIndeces[4] = {};		// 참조하는 본의 인덱스의 범위는 최대128개로 일단처리하자
	float BlendWeights[4] = {};	// 가중치 총합은 1이 되어야한다.

	void AddBoneData(int boneIndex, float weight)
	{
		// 적어도 하나는 데이터가 비어있어야 한다.
		assert(BlendWeights[0] == 0.0f || BlendWeights[1] == 0.0f ||
			BlendWeights[2] == 0.0f || BlendWeights[3] == 0.0f);
		for (int i = 0; i < 4; i++)
		{
			if (BlendWeights[i] == 0.0f)
			{
				BlendIndeces[i] = boneIndex;
				BlendWeights[i] = weight;
				return;
			}
		}
	}
};

struct BoneReference
{
	std::string NodeName;		 // 노드 이름
	int BoneIndex = -1;			 // 본 인덱스
	Math::Matrix OffsetMatrix;	 // 본기준 메시의 OffsetMatrix
};



struct aiMesh;
class Bone;

class SkeletalMeshResource : public StaticMeshResource
{
public:
	SkeletalMeshResource();
	~SkeletalMeshResource();

	std::vector<BoneWeightVertex>	m_Vertices;
	std::vector<BoneReference>		m_BoneReferences;
	void Create(aiMesh* mesh, SkeletonResource* skeleton);
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
	SkeletonResource m_Skeleton;

	std::vector<std::shared_ptr<Animation>> m_Animations;

	bool Create(std::string filePath);
	
	Material* GetMeshMaterial(UINT index);

	void AddAnimation(std::shared_ptr<Animation> animation);
};