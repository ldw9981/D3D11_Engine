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
	std::string NodeName;		 
	int BoneIndex = -1;			 // �� �ε���
	Math::Matrix* NodeWorldMatrixPtr;
};



struct aiMesh;
struct Skeleton;
class Node;
class Mesh
{
public:
	Mesh();
	~Mesh();
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

class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	std::vector<Vertex>				m_Vertices;
	std::vector<WORD>				m_Indices;	

	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh�� ��������� ���� ����� ������
	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_IndexCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;					// �޽� �̸�.

	template<class T>
	void CreateVertexBuffer(ID3D11Device* device, T* vertices, UINT vertexCount);
	void CreateIndexBuffer(ID3D11Device* device, WORD* indies, UINT indexCount);
	virtual void Create(ID3D11Device* device, aiMesh* mesh, Skeleton* skeleton);
	// ���� ���� ��尡 ������ World�� �����͸� ����
	void SetNodeWorldPtr(Math::Matrix* world) { m_pNodeWorldTransform = world; }
	virtual void UpdateNodeInstancePtr(Node* pRootNode, Skeleton* skeleton);
	void Render(ID3D11DeviceContext* deviceContext);
};


class SkinnedMesh:public StaticMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();
public:
	std::vector<BoneWeightVertex>	m_BoneWeightVertices;
	std::vector<BoneReference>		m_BoneReferences;		
	
	virtual void Create(ID3D11Device* device, aiMesh* mesh, Skeleton* skeleton);
	void UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, Skeleton* skeleton);	
	virtual void UpdateNodeInstancePtr(Node* pRootNode, Skeleton* skeleton);
};