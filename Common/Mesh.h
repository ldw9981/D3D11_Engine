#pragma once
#include "D3DRenderManager.h"

// ���� ����.
struct Vertex
{
	Math::Vector3 Position;		// ���� ��ġ ����.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
};

struct BoneWeightVertex
{
	Math::Vector3 Position;		// ���� ��ġ ����.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
	// ����޴� ������ �ִ�4���� �����Ѵ�.
	int BlendIndeces[4] = {};		// �����ϴ� ���� �ε����� ������ �ִ�128���� �ϴ�ó������
	float BlendWeights[4] = {};	// ����ġ ������ 1�� �Ǿ���Ѵ�.

	void AddBoneData(int boneIndex, float weight)
	{
		// ��� �ϳ��� �����Ͱ� ����־�� �Ѵ�.
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
	std::string NodeName;		 // ��� �̸�
	int BoneIndex = -1;			 // �� �ε���
	Math::Matrix OffsetMatrix;	 // ������ �޽��� OffsetMatrix
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
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;					// �޽� �̸�.	

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
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh�� ��������� ���� ����� ������
	
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
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;					// �޽� �̸�.	

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
	std::vector<Math::Matrix*>		m_BoneReferences;	// ���� �ش�Ǵ� ���MATRIX�� �����͸� �����Ѵ�.
	Math::Matrix* m_pNodeWorldTransform = nullptr;

	void Create(SkeletalMeshResource* pMeshResource, SkeletonInfo* skeleton, Node* pRootNode, Material* pMaterial);
	void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, SkeletonInfo* skeleton);

	
	virtual void Render(ID3D11DeviceContext* deviceContext);
};


