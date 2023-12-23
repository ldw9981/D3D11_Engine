#include "pch.h"
#include "StaticMeshInstance.h"
#include "StaticMeshResource.h"
#include "Node.h"


StaticMeshInstance::StaticMeshInstance()
{
}

StaticMeshInstance::~StaticMeshInstance()
{

}


void StaticMeshInstance::Create(StaticMeshResource* pMeshResource, Node* pRootNode, Material* pMaterial)
{
	m_pMeshResource = pMeshResource;
	m_pMaterial = pMaterial;
	m_pNodeWorldTransform = &pRootNode->m_World;
}

void StaticMeshInstance::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetIndexBuffer(m_pMeshResource->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &m_pMeshResource->m_pVertexBuffer, &m_pMeshResource->m_VertexBufferStride, &m_pMeshResource->m_VertexBufferOffset);
	deviceContext->DrawIndexed(m_pMeshResource->m_FaceCount*3, 0, 0);
}
