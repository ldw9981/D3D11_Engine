#include "pch.h"
#include "MeshResource.h"
#include "Helper.h"
#include "D3DRenderManager.h"

MeshResource::MeshResource()
{

}

MeshResource::~MeshResource()
{

}

void MeshResource::CreateIndexBuffer(Face* faces, UINT faceCount)
{
	// �ε��� ���� ����.
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

