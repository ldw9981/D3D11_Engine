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
	Math::Matrix* NodeWorldMatrixPtr = nullptr;
};



struct aiMesh;
struct Skeleton;
class Node;



class MeshInstance
{
public:
	MeshInstance();
	~MeshInstance();
public:	
	std::vector<Vertex>				m_Vertices;
	std::vector<BoneWeightVertex>	m_BoneWeightVertices;
	std::vector<WORD>				m_Indices;

	std::vector<BoneReference>		m_BoneReferences;

	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh�� ��������� ���� ����� ������
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertexCount=0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;					// �޽� �̸�.

	void CreateVertexBuffer(ID3D11Device* device, Vertex* vertices, UINT vertexCount);
	void CreateBoneWeightVertexBuffer(ID3D11Device* device, BoneWeightVertex* vertices, UINT vertexCount);

	void CreateIndexBuffer(ID3D11Device* device, WORD* indies, UINT indexCount);
	void Create(ID3D11Device* device,aiMesh* mesh, Skeleton* skeleton);
	// ���� ���� ��尡 ������ World�� �����͸� ����
	void SetNodeWorldPtr(Math::Matrix* world) { m_pNodeWorldTransform = world; }
	void UpdateNodeInstancePtr(Node* pRootNode, Skeleton* skeleton);
	void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, Skeleton* skeleton);
	bool IsSkeletalMesh() { return !m_BoneReferences.empty(); }		
	void Render(ID3D11DeviceContext* deviceContext);
};


class SkeletalMeshResource
{
public:
	SkeletalMeshResource();
	~SkeletalMeshResource();

	std::vector<BoneWeightVertex>	m_BoneWeightVertices;
	std::vector<WORD>				m_Indices;	

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;					// �޽� �̸�.	

	void Create(ID3D11Device* device, aiMesh* mesh, Skeleton* skeleton);

	void CreateBoneWeightVertexBuffer(ID3D11Device* device, BoneWeightVertex* vertices, UINT vertexCount);

	void CreateIndexBuffer(ID3D11Device* device, WORD* indies, UINT indexCount);
};


class SkeletalMeshInstance
{
public:
	SkeletalMeshInstance();
	~SkeletalMeshInstance();
public:

	std::shared_ptr<SkeletalMeshResource> m_pSkeletalMeshResource;
	
	std::vector<BoneReference>		m_BoneReferences;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh�� ��������� ���� ����� ������
	Material* m_pMaterial = nullptr;

	void Create(std::string key, aiMesh* mesh, Skeleton* skeleton,Node* pRootNode );
	// ���� ���� ��尡 ������ World�� �����͸� ����
	void SetNodeWorldPtr(Math::Matrix* world) { m_pNodeWorldTransform = world; }
	void SetMaterial(Material* material) { m_pMaterial = material; }
	void UpdateNodeInstancePtr(Node* pRootNode, Skeleton* skeleton);
	void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, Skeleton* skeleton);

	void Render(ID3D11DeviceContext* deviceContext);
};


