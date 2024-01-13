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
	UINT m_VertexBufferStride = 0;						// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;						// 버텍스 버퍼의 오프셋.
	UINT m_FaceCount = 0;				// 인덱스 개수.
	UINT m_MaterialIndex = 0;			// 메테리얼 인덱스.
	std::string m_Name;					// 메쉬 이름.	

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

		// 버텍스 버퍼 정보
		m_VertexCount = vertexCount;
		m_VertexBufferStride = sizeof(T);
		m_VertexBufferOffset = 0;
	}
};

