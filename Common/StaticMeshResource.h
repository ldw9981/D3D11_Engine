#pragma once
#include "Material.h"
#include "Helper.h"
#include "MeshResource.h"

// ���� ����.
struct Vertex
{
	Math::Vector3 Position;		// ���� ��ġ ����.	
	Math::Vector2 TexCoord;
	Math::Vector3 Normal;
	Math::Vector3 Tangent;
	Math::Vector3 BiTangent;
};



// �ϳ��� StaticMesh�� �ǹ��ϴ� ����������
class StaticMeshResource : public MeshResource
{
public:
	StaticMeshResource();
	virtual ~StaticMeshResource();

	std::vector<Vertex>		m_Vertices;
	void Create(aiMesh* mesh);
};


// �ϳ��� StaticMesh FBX�� �ǹ��ϴ� ����������, ���� �޽��� ���� �� �ִ�.
class StaticMeshSceneResource
{
public:
	StaticMeshSceneResource() {}
	~StaticMeshSceneResource() {}

	Math::Vector3 m_AABBmin;
	Math::Vector3 m_AABBmax;
	Math::Vector3 m_BoundingBoxMin;	// ȸ���� ����� ������AABB
	Math::Vector3 m_BoundingBoxMax;	// ȸ���� ����� ������AABB
	std::vector<StaticMeshResource> m_StaticMeshResources;
	std::vector<Material> m_Materials;

	bool Create(std::string filePath);
	Material* GetMeshMaterial(UINT index);

	void GetAABB(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents);
	void GetBoundingBox(DirectX::XMFLOAT3& center, DirectX::XMFLOAT3& Extents);
};