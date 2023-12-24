#pragma once


// �������� �ʿ��� ��������, ���� �޽��� ����� Transform, ����� ���͸���
class StaticMeshResource;
class Bone;
class Material;
class StaticMeshInstance 
{
public:
	friend class D3DRenderManager;		// D3DRenderManager�� ����� ���Ѿ��� ���ٰ����ϰ� �Ѵ�.

	StaticMeshInstance();
	~StaticMeshInstance();
public:
	StaticMeshResource* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// StaticMesh�� ��������� ���� ����� ������

	void Create(StaticMeshResource* pMeshResource, Math::Matrix* pWorld, Material* pMaterial);
	virtual void Render(ID3D11DeviceContext* deviceContext);
};