#pragma once


// 렌더링에 필요한 정보모음, 단일 메쉬와 연결된 Transform, 연결된 머터리얼
class StaticMeshResource;
class Node;
class Material;
class StaticMeshInstance 
{
public:
	StaticMeshInstance();
	~StaticMeshInstance();
public:
	StaticMeshResource* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh의 월드행렬을 가진 노드의 포인터

	void Create(StaticMeshResource* pMeshResource, Node* pRootNode, Material* pMaterial);
	virtual void Render(ID3D11DeviceContext* deviceContext);
};