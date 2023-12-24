#include "pch.h"
#include "SkeletalMeshResource.h"
#include "Helper.h"
#include "D3DRenderManager.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include "ResourceManager.h"
#include "TimeSystem.h"


SkeletalMeshResource::SkeletalMeshResource()
{

}

SkeletalMeshResource::~SkeletalMeshResource()
{
	
}

void SkeletalMeshResource::Create(aiMesh* mesh, SkeletonResource* skeleton)
{
	//Skeletal Mesh
	m_Vertices.resize(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		m_Vertices[i].Position = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		m_Vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_Vertices[i].TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_Vertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		m_Vertices[i].BiTangent = Vector3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
	}
	UINT meshBoneCount = mesh->mNumBones;	// 메쉬와 연결된 본개수
	m_BoneReferences.resize(meshBoneCount); // 본 연결 정보 컨테이너 크기 조절
	for (UINT i = 0; i < meshBoneCount; ++i)
	{
		aiBone* pAiBone = mesh->mBones[i];

		// 스켈레톤에서 본정보를 찾는다.
		UINT boneIndex = skeleton->GetBoneIndexByBoneName(pAiBone->mName.C_Str());
		assert(boneIndex != -1);
		BoneInfo* pBone = skeleton->GetBone(boneIndex);
		assert(pBone != nullptr);

		m_BoneReferences[i].NodeName = pAiBone->mName.C_Str();
		m_BoneReferences[i].BoneIndex = boneIndex;
		m_BoneReferences[i].OffsetMatrix = Math::Matrix(&pAiBone->mOffsetMatrix.a1).Transpose();

		// 본과 연결된 버텍스들을 처리
		for (UINT j = 0; j < pAiBone->mNumWeights; ++j)
		{
			UINT vertexID = pAiBone->mWeights[j].mVertexId;
			float weight = pAiBone->mWeights[j].mWeight;
			m_Vertices[vertexID].AddBoneData(boneIndex, weight);
		}
	}
	CreateVertexBuffer<BoneWeightVertex>(&m_Vertices[0], (UINT)m_Vertices.size());


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


bool SkeletalMeshSceneResource::Create(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));	
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ 노드 생성안함
	unsigned int importFlags = aiProcess_Triangulate | // 삼각형으로 변환
		aiProcess_GenNormals |	// 노말 생성/
		aiProcess_GenUVCoords |		// UV 생성
		aiProcess_CalcTangentSpace |  // 탄젠트 생성
		aiProcess_LimitBoneWeights |	// 본의 영향을 받는 정점의 최대 개수를 4개로 제한
		aiProcess_GenBoundingBoxes | // 바운딩 박스 생성
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

	m_Skeleton.Create(scene);

	m_Materials.resize(scene->mNumMaterials);
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		m_Materials[i].Create(scene->mMaterials[i]);
	}

	m_SkeletalMeshResources.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		m_SkeletalMeshResources[i].Create(scene->mMeshes[i], &m_Skeleton);
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

	// SceneResource의 기본 애니메이션 추가한다.

	assert(scene->mNumAnimations < 2); // 애니메이션은 없거나 1개여야한다. 
	// 노드의 애니메이션을 하나로 합치는 방법은 FBX export에서 NLA스트립,모든 액션 옵션을 끕니다.
	if (scene->HasAnimations())
	{
		const aiAnimation* pAiAnimation = scene->mAnimations[0];
		// 채널수는 aiAnimation 안에서 애니메이션 정보를  표현하는 aiNode의 개수이다.
		assert(pAiAnimation->mNumChannels > 1); // 애니메이션이 있다면 aiNode 는 하나 이상 있어야한다.

		shared_ptr<AnimationResource> anim = make_shared<AnimationResource>();		
		anim->Create(filePath, pAiAnimation);
		m_Animations.push_back(anim);
	}
	importer.FreeScene();
	return true;
}



void SkeletalMeshSceneResource::AddAnimation(std::shared_ptr<AnimationResource> animation)
{
	m_Animations.push_back(animation);
}

Material* SkeletalMeshSceneResource::GetMeshMaterial(UINT index)
{
	assert(index < m_Materials.size());
	UINT mindex = m_SkeletalMeshResources[index].m_MaterialIndex;
	assert(mindex < m_Materials.size());
	return &m_Materials[mindex];
}