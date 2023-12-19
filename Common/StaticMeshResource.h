#pragma once
#include "Material.h"

// 정점 선언.
struct Vertex
{
	Math::Vector3 Position;		// 정점 위치 정보.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
};


// 하나의 StaticMesh를 의미하는 공유데이터
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
	void GetAABB(Math::Vector3& min, Math::Vector3& max);
};

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