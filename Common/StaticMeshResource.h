#pragma once
#include "Material.h"
#include "Helper.h"
#include "MeshResource.h"

// 정점 선언.
struct Vertex
{
	Math::Vector3 Position;		// 정점 위치 정보.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
	Math::Vector3 BiTangent;
};



// 하나의 StaticMesh를 의미하는 공유데이터
class StaticMeshResource : public MeshResource
{
public:
	StaticMeshResource();
	virtual ~StaticMeshResource();

	std::vector<Vertex>		m_Vertices;
	void Create(aiMesh* mesh);
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