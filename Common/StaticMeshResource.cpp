#include "pch.h"
#include "StaticMeshResource.h"
#include "Helper.h"
#include "D3DRenderManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include "ResourceManager.h"
#include "TimeSystem.h"


using namespace DirectX;



StaticMeshResource::StaticMeshResource()
{
}

StaticMeshResource::~StaticMeshResource()
{

}

void StaticMeshResource::Create(aiMesh* mesh)
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
		m_Vertices[i].BiTangent = Vector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
	}
	CreateVertexBuffer(&m_Vertices[0], (UINT)m_Vertices.size());

	// 인덱스 정보 생성
	m_Faces.resize(mesh->mNumFaces);
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);
		m_Faces[i].i0 = mesh->mFaces[i].mIndices[0];
		m_Faces[i].i1 = mesh->mFaces[i].mIndices[1];
		m_Faces[i].i2 = mesh->mFaces[i].mIndices[2];
	}
	CreateIndexBuffer(&m_Faces[0], (UINT)m_Faces.size());
}




bool StaticMeshSceneResource::Create(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));

	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ 노드 생성안함

	unsigned int importFlags = aiProcess_Triangulate | // 삼각형으로 변환
		aiProcess_GenNormals |	// 노말 생성/
		aiProcess_GenUVCoords |		// UV 생성
		aiProcess_CalcTangentSpace |  // 탄젠트 생성			
		aiProcess_GenBoundingBoxes | // 바운딩 박스 생성
		aiProcess_PreTransformVertices | // 노드의 변환행렬을 적용한 버텍스 생성	
		aiProcess_ConvertToLeftHanded;	// 왼손 좌표계로 변환

	const aiScene* scene = importer.ReadFile(filePath, importFlags);
	if (!scene) {
		LOG_ERRORA("Error loading file: %s", importer.GetErrorString());
		return false;
	}

	//데이터 확인용 게임에 필요없음
	aiMetadata* pAiMetaData = scene->mMetaData;
	std::vector<MetaData> metaDataList;
	if (pAiMetaData)
	{	// Iterate through the metadata
		for (unsigned int i = 0; i < pAiMetaData->mNumProperties; i++)
		{
			MetaData& data = metaDataList.emplace_back();
			data.Name = pAiMetaData->mKeys[i].C_Str();
			data.SetData(pAiMetaData->mValues[i]);
		}
	}

	m_Materials.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		m_Materials[i].Create(scene->mMaterials[i]);
	}

	m_StaticMeshResources.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		m_StaticMeshResources[i].Create(scene->mMeshes[i]);
	}

	m_AABBmin = Math::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_AABBmax = Math::Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* pMesh = scene->mMeshes[i];
		Math::Vector3 meshMin = Math::Vector3(pMesh->mAABB.mMin.x, pMesh->mAABB.mMin.y, pMesh->mAABB.mMin.z);
		Math::Vector3 meshMax = Math::Vector3(pMesh->mAABB.mMax.x, pMesh->mAABB.mMax.y, pMesh->mAABB.mMax.z);

		m_AABBmin = Math::Vector3::Min(m_AABBmin, meshMin);
		m_AABBmax = Math::Vector3::Max(m_AABBmax, meshMax);
	}

	importer.FreeScene();	
	return true;
}

Material* StaticMeshSceneResource::GetMeshMaterial(UINT index)
{
	assert(index < m_StaticMeshResources.size());
	UINT mindex = m_StaticMeshResources[index].m_MaterialIndex;
	assert(mindex < m_Materials.size());
	return &m_Materials[mindex];
}