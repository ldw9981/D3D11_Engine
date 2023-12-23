#pragma once
#include "Material.h"
#include "Helper.h"

// 정점 선언.
struct Vertex
{
	Math::Vector3 Position;		// 정점 위치 정보.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
	Math::Vector3 BiTangent;
};

struct Face
{
	UINT i0, i1, i2;
};


// 하나의 StaticMesh를 의미하는 공유데이터
class StaticMeshResource
{
public:
	StaticMeshResource();
	virtual ~StaticMeshResource();

	std::vector<Vertex>		m_Vertices;
	std::vector<Face>		m_Faces;

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;						// 버텍스 버퍼의 오프셋.
	UINT m_FaceCount = 0;				// 인덱스 개수.
	UINT m_MaterialIndex = 0;			// 메테리얼 인덱스.
	std::string m_Name;					// 메쉬 이름.	

	void Create(aiMesh* mesh);
	template<typename T>
	void CreateVertexBuffer(T* vertices, UINT vertexCount);
	void CreateIndexBuffer(Face* faces, UINT faceCount);
};

template<typename T>
void StaticMeshResource::CreateVertexBuffer(T* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(T) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(T);
	m_VertexBufferOffset = 0;
}

// 하나의 StaticMesh FBX를 의미하는 공유데이터, 여러 메쉬를 가질 수 있다.
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