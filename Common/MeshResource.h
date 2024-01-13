#pragma once
#include "D3DRenderManager.h"
#include "Helper.h"

struct Face
{
	UINT i0, i1, i2;
};


struct Vertex;
struct BoneWeightVertex;
class MeshResource
{
public:
	MeshResource();
	virtual ~MeshResource();

	std::vector<Face>		m_Faces;

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;						// ���ؽ� �ϳ��� ũ��.
	UINT m_VertexBufferOffset = 0;						// ���ؽ� ������ ������.
	UINT m_FaceCount = 0;				// �ε��� ����.
	UINT m_MaterialIndex = 0;			// ���׸��� �ε���.
	std::string m_Name;					// �޽� �̸�.	

	void CreateIndexBuffer(Face* faces, UINT faceCount);
	template<typename T>
	void CreateVertexBuffer(T* vertices, UINT vertexCount)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = sizeof(T) * vertexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vbData = {};
		vbData.pSysMem = vertices;
		HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, m_pVertexBuffer.GetAddressOf()));

		// ���ؽ� ���� ����
		m_VertexCount = vertexCount;
		m_VertexBufferStride = sizeof(T);
		m_VertexBufferOffset = 0;
	}
};

