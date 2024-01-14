#include "pch.h"
#include "MeshResource.h"
#include "Helper.h"
#include "D3DRenderManager.h"
#include "GeometryGenerator.h"

MeshResource::MeshResource()
{

}

MeshResource::~MeshResource()
{

}

void MeshResource::CreateIndexBuffer(Face* faces, UINT faceCount)
{
	// 인덱스 개수 저장.
	m_FaceCount = faceCount;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Face) * faceCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = faces;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &ibData, m_pIndexBuffer.GetAddressOf()));
}

void MeshResource::Create(GeometryData& in)
{
	CreateVertexBuffer<Vertex>(&in.vertices[0], in.vertices.size());
	CreateIndexBuffer(&in.indices[0],in.indices.size());	
}

