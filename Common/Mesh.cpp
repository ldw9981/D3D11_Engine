#include "pch.h"
#include "Mesh.h"
#include "Helper.h"
#include "Node.h"
#include "Skeleton.h"
#include "ResourceManager.h"

using namespace DirectX;

MeshInstance::MeshInstance()
{
}

MeshInstance::~MeshInstance()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void MeshInstance::CreateVertexBuffer(ID3D11Device* device, Vertex* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(device->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(Vertex);
	m_VertexBufferOffset = 0;
}


void MeshInstance::CreateIndexBuffer(ID3D11Device* device, WORD* indices, UINT indexCount)
{
	// 인덱스 개수 저장.
	m_IndexCount = indexCount;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(WORD) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	HR_T(device->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));
}

void MeshInstance::Create(ID3D11Device* device, aiMesh* mesh, Skeleton* skeleton)
{
	m_MaterialIndex = mesh->mMaterialIndex;
	m_Name = mesh->mName.C_Str();

	
	//Static Mesh
	m_Vertices.resize(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		m_Vertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		m_Vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_Vertices[i].TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_Vertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}
	CreateVertexBuffer(device, &m_Vertices[0], (UINT)m_Vertices.size());
	

	// 인덱스 정보 생성
	m_Indices.resize(mesh->mNumFaces * 3);
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		m_Indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		m_Indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		m_Indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
	CreateIndexBuffer(device, &m_Indices[0], (UINT)m_Indices.size());
}

void MeshInstance::UpdateNodeInstancePtr(Node* pRootNode, Skeleton* skeleton)
{

		Bone* pBone = skeleton->GetBone(skeleton->GetBoneIndexByMeshName(m_Name));
		assert(pBone != nullptr);

		Node* pNode = pRootNode->FindNode(pBone->Name);
		assert(pNode != nullptr);
		m_pNodeWorldTransform = &pNode->m_World;


}


void MeshInstance::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &m_VertexBufferOffset);
	deviceContext->DrawIndexed(m_IndexCount, 0, 0);
}



SkeletalMeshInstance::SkeletalMeshInstance()
{
}

SkeletalMeshInstance::~SkeletalMeshInstance()
{

}


void SkeletalMeshInstance::Create(SkeletalMeshResource* pSkeletalMeshResource,  Skeleton* pSkeleton, Node* pRootNode, Material* pMaterial)
{	
	m_pSkeletalMeshResource = pSkeletalMeshResource;
	m_pMaterial = pMaterial;
	size_t meshBoneCount = m_pSkeletalMeshResource->m_BoneReferences.size();	// 메쉬와 연결된 본개수
	m_BoneReferences.resize(meshBoneCount); // 본 연결 정보 컨테이너 크기 조절		
	for (size_t i = 0; i < meshBoneCount; ++i)
	{
		std::string& name = m_pSkeletalMeshResource->m_BoneReferences[i].NodeName;
		Node* pNode = pRootNode->FindNode(name);
		assert(pNode != nullptr);
		m_BoneReferences[i] = &pNode->m_World;
	}
}
void SkeletalMeshInstance::UpdateMatrixPallete(CB_MatrixPalette* pMatrixPallete, Skeleton* skeleton)
{
	assert(m_BoneReferences.size() == m_pSkeletalMeshResource->m_BoneReferences.size());
	size_t meshBoneCount = m_pSkeletalMeshResource->m_BoneReferences.size();	// 메쉬와 연결된 본개수
	for (size_t i = 0; i < meshBoneCount; ++i)
	{
		Math::Matrix& BoneNodeWorldMatrix = *m_BoneReferences[i];
		// HLSL 상수버퍼에 업데이트할때 바로 복사할수있도록 전치해서 저장한다.
		
		BoneReference& br = m_pSkeletalMeshResource->m_BoneReferences[i];
		pMatrixPallete->Array[br.BoneIndex] = (br.OffsetMatrix * BoneNodeWorldMatrix).Transpose();
	}
}

void SkeletalMeshInstance::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetIndexBuffer( m_pSkeletalMeshResource->m_pIndexBuffer , DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &m_pSkeletalMeshResource->m_pVertexBuffer, &m_pSkeletalMeshResource->m_VertexBufferStride, &m_pSkeletalMeshResource->m_VertexBufferOffset);
	deviceContext->DrawIndexed(m_pSkeletalMeshResource->m_IndexCount, 0, 0);
}

SkeletalMeshResource::SkeletalMeshResource()
{	

}

SkeletalMeshResource::~SkeletalMeshResource()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

void SkeletalMeshResource::Create(aiMesh* mesh, Skeleton* skeleton)
{
	//Skeletal Mesh
	m_BoneWeightVertices.resize(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		m_BoneWeightVertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		m_BoneWeightVertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_BoneWeightVertices[i].TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_BoneWeightVertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

	}
	UINT meshBoneCount = mesh->mNumBones;	// 메쉬와 연결된 본개수
	m_BoneReferences.resize(meshBoneCount); // 본 연결 정보 컨테이너 크기 조절
	for (UINT i = 0; i < meshBoneCount; ++i)
	{
		aiBone* pAiBone = mesh->mBones[i];

		// 스켈레톤에서 본정보를 찾는다.
		UINT boneIndex = skeleton->GetBoneIndexByBoneName(pAiBone->mName.C_Str());       
		assert(boneIndex != -1);
		Bone* pBone = skeleton->GetBone(boneIndex);
		assert(pBone != nullptr);		
				
		m_BoneReferences[i].NodeName = pAiBone->mName.C_Str();
		m_BoneReferences[i].BoneIndex = boneIndex;
		m_BoneReferences[i].OffsetMatrix = Math::Matrix(&pAiBone->mOffsetMatrix.a1).Transpose();

		// 본과 연결된 버텍스들을 처리
		for (UINT j = 0; j < pAiBone->mNumWeights; ++j)
		{
			UINT vertexID = pAiBone->mWeights[j].mVertexId;
			float weight = pAiBone->mWeights[j].mWeight;
			m_BoneWeightVertices[vertexID].AddBoneData(boneIndex, weight);
		}
	}
	CreateBoneWeightVertexBuffer(&m_BoneWeightVertices[0], (UINT)m_BoneWeightVertices.size());
	

	// 인덱스 정보 생성
	m_Indices.resize(mesh->mNumFaces * 3);
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		m_Indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		m_Indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		m_Indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}
	CreateIndexBuffer(&m_Indices[0], (UINT)m_Indices.size());
}

void SkeletalMeshResource::CreateBoneWeightVertexBuffer( BoneWeightVertex* vertices, UINT vertexCount)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(BoneWeightVertex) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, &m_pVertexBuffer));

	// 버텍스 버퍼 정보
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(BoneWeightVertex);
	m_VertexBufferOffset = 0;
}

void SkeletalMeshResource::CreateIndexBuffer(WORD* indices, UINT indexCount)
{
	// 인덱스 개수 저장.
	m_IndexCount = indexCount;

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(WORD) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = indices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &ibData, &m_pIndexBuffer));
}
