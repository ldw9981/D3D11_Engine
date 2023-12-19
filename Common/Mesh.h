#pragma once
#include "D3DRenderManager.h"

// 정점 선언.
struct Vertex
{
	Math::Vector3 Position;		// 정점 위치 정보.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
};

struct BoneWeightVertex
{
	Math::Vector3 Position;		// 정점 위치 정보.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
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
struct SkeletonInfo;
class Node;

class StaticMeshResource
{
public:
	StaticMeshResource();
	~StaticMeshResource();

	std::vector<Vertex>		m_Vertices;
	std::vector<WORD>		m_Indices;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;						// 버텍스 버퍼의 오프셋.
	UINT m_IndexCount = 0;				// 인덱스 개수.
	UINT m_MaterialIndex = 0;			// 메테리얼 인덱스.
	std::string m_Name;					// 메쉬 이름.	

	Math::Vector3 m_AABBmin;
	Math::Vector3 m_AABBmax;

	void Create(aiMesh* mesh);
	void CreateVertexBuffer(Vertex* vertices, UINT vertexCount);
	void CreateIndexBuffer(WORD* indies, UINT indexCount);
};



class StaticMeshInstance : public IRenderable
{
public:
	StaticMeshInstance();
	~StaticMeshInstance();
public:	
	StaticMeshResource* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh의 월드행렬을 가진 노드의 포인터
	
	void Create(StaticMeshResource* pMeshResource,Node* pRootNode, Material* pMaterial);	
	virtual void Render(ID3D11DeviceContext* deviceContext);
};


class SkeletalMeshResource
{
public:
	SkeletalMeshResource();
	~SkeletalMeshResource();

	std::vector<BoneWeightVertex>	m_Vertices;
	std::vector<WORD>				m_Indices;
	std::vector<BoneReference>		m_BoneReferences;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;						// 버텍스 버퍼의 오프셋.
	UINT m_IndexCount = 0;				// 인덱스 개수.
	UINT m_MaterialIndex = 0;			// 메테리얼 인덱스.
	std::string m_Name;					// 메쉬 이름.	

	Math::Vector3 m_AABBmin;
	Math::Vector3 m_AABBmax;

	void Create(aiMesh* mesh, SkeletonInfo* skeleton);
	void CreateVertexBuffer(BoneWeightVertex* vertices, UINT vertexCount);
	void CreateIndexBuffer(WORD* indies, UINT indexCount);
};


class SkeletalMeshInstance: public IRenderable
{
public:
	SkeletalMeshInstance();
	~SkeletalMeshInstance();
public:

	SkeletalMeshResource* m_pMeshResource;
	Material* m_pMaterial = nullptr;
	std::vector<Math::Matrix*>		m_BoneReferences;	// 본에 해당되는 노드MATRIX의 포인터를 저장한다.
	Math::Matrix* m_pNodeWorldTransform = nullptr;

	void Create(SkeletalMeshResource* pMeshResource, SkeletonInfo* skeleton, Node* pRootNode, Material* pMaterial);
	void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, SkeletonInfo* skeleton);

	
	virtual void Render(ID3D11DeviceContext* deviceContext);
};


