#pragma once


// 렌더링에 필요한 정보모음, 단일 메쉬와 연결된 Transform, 연결된 머터리얼
class StaticMeshResource;
class Bone;
class Material;
class StaticMeshInstance 
{
public:
	friend class D3DRenderManager;		// D3DRenderManager는 멤버에 제한없이 접근가능하게 한다.

	StaticMeshInstance();
	~StaticMeshInstance();
public:
	StaticMeshResource* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh의 월드행렬을 가진 노드의 포인터

	void Create(StaticMeshResource* pMeshResource, Math::Matrix* pWorld, Material* pMaterial);
	virtual void Render(ID3D11DeviceContext* deviceContext);
};