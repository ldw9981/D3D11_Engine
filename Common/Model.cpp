#include "pch.h"
#include "Model.h"
#include "Helper.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include "ResourceManager.h"
#include "TimeSystem.h"


bool StaticMeshModel::ReadSceneResourceFromFBX(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath.c_str());
	GameTimer timer;
	timer.Tick();
	// ���ҽ� �Ŵ������� �����´�.
	m_SceneResource = ResourceManager::Instance->CreateStaticMeshSceneResource(filePath);
	if (!m_SceneResource) {
		return false;
	}

	//CreateHierachy(&m_SceneResource->m_Skeleton);	//StaticMesh�� ���������� ��� ���ϱ�� �Ѵ�.

	// �ν��Ͻ� ����
	m_MeshInstances.resize(m_SceneResource->m_StaticMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_StaticMeshResources.size(); i++)
	{
		m_MeshInstances[i].Create(&m_SceneResource->m_StaticMeshResources[i], // mesh resource		
			this,	// root node
			m_SceneResource->GetMeshMaterial(i));		//material resource 
	}

	timer.Tick();
	LOG_MESSAGEA("Complete file: %s %f", filePath.c_str(), timer.DeltaTime());
	return true;
}

Material* StaticMeshModel::GetMaterial(UINT index)
{
	assert(index < m_SceneResource->m_Materials.size());
	return &m_SceneResource->m_Materials[index];
}

void StaticMeshModel::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void StaticMeshModel::SetWorldTransform(const Math::Matrix& transform)
{
	m_Local = transform;
}


void MetaData::SetData(const aiMetadataEntry& entry)
{
	// Check the data type of the metadata entry
	switch (Type)
	{
	case AI_BOOL:
		break;
	case AI_INT32:
		metadataInt = *reinterpret_cast<const int*>(entry.mData);
		break;
	case AI_UINT64:
		break;
	case AI_FLOAT:
		metadataFloat = *reinterpret_cast<const float*>(entry.mData);
		break;
	case AI_DOUBLE:
		break;
	case AI_AISTRING:
		metadataStr = std::string((char*)entry.mData);
		break;
	case AI_AIVECTOR3D:
		metadataVector[0] = reinterpret_cast<const aiVector3D*>(entry.mData)->x;
		metadataVector[1] = reinterpret_cast<const aiVector3D*>(entry.mData)->y;
		metadataVector[2] = reinterpret_cast<const aiVector3D*>(entry.mData)->z;
		break;
	case AI_META_MAX:
		break;
	default:
		break;
	}
}



bool SkeletalMeshModel::ReadSceneResourceFromFBX(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));	
	LOG_MESSAGEA("Loading file: %s", filePath.c_str());
	
	GameTimer timer;
	timer.Tick();
	// ���ҽ� �Ŵ������� �����´�.
	m_SceneResource = ResourceManager::Instance->CreateSkeletalMeshSceneResource(filePath);	
	if (!m_SceneResource) {
		return false;
	}

	// ���ҽ��� �ν��Ͻ� ó���Ѵ�.
	CreateHierachy(&m_SceneResource->m_Skeleton);	//�������� ����	

	m_MeshInstances.resize(m_SceneResource->m_SkeletalMeshResources.size());
	for (UINT i = 0; i < m_SceneResource->m_SkeletalMeshResources.size(); i++)
	{
		m_MeshInstances[i].Create(   &m_SceneResource->m_SkeletalMeshResources[i], // mesh resource
			&m_SceneResource->m_Skeleton,	 // skeleton resource
			this,	// root node
			m_SceneResource->GetMeshMaterial(i));		//material resource 
	}
	UpdateNodeAnimationReference(0);	// �� ����� �ִϸ��̼� �������� ����	
	timer.Tick();
	LOG_MESSAGEA("Complete file: %s %f", filePath.c_str(),timer.DeltaTime());
	return true;
}

bool SkeletalMeshModel::ReadAnimationOnlyFromFBX(std::string filePath)
{
	auto it = std::find_if(m_SceneResource->m_Animations.begin(), m_SceneResource->m_Animations.end(),
		[filePath](const Animation& node) {
			return node.FilePath == filePath;
		});

	if (it != m_SceneResource->m_Animations.end())
	{
		return false;
	}	
	return m_SceneResource->AddAnimation(filePath);
}

Material* SkeletalMeshModel::GetMaterial(UINT index)
{
	assert(index < m_SceneResource->m_Materials.size());
	return &m_SceneResource->m_Materials[index];
}
void SkeletalMeshModel::Update(float deltaTime)
{
	if (!m_SceneResource->m_Animations.empty())
	{
		m_AnimationProressTime += deltaTime;
		m_AnimationProressTime = fmod(m_AnimationProressTime, m_SceneResource->m_Animations[m_AnimationIndex].Duration);
		
	}
	__super::Update(deltaTime);	
}

void SkeletalMeshModel::UpdateNodeAnimationReference(UINT index)
{
	assert(index < m_SceneResource->m_Animations.size());
	Animation& animation = m_SceneResource->m_Animations[index];
	for (size_t i = 0; i < animation.NodeAnimations.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation.NodeAnimations[i];
		Node* pNode = FindNode(nodeAnimation.NodeName);
		assert(pNode!=nullptr);
		pNode->m_pNodeAnimation = &animation.NodeAnimations[i];
	}
}

void SkeletalMeshModel::SetWorldTransform(const Math::Matrix& transform)
{
	m_Local = transform;
}

void SkeletalMeshModel::PlayAnimation(UINT index)
{
	assert(index < m_SceneResource->m_Animations.size());
	m_AnimationIndex = index;
	m_AnimationProressTime = 0.0f;
	UpdateNodeAnimationReference(index);
}

void SkeletalMeshModel::CreateHierachy(SkeletonInfo* skeleton)
{
	UINT count = skeleton->GetBoneCount();

	BoneInfo* pRootBone = skeleton->GetBone(0);
	m_Name = pRootBone->Name;
	m_Children.reserve(pRootBone->NumChildren);

	// 0�� ��Ʈ�� �����̳��̹Ƿ� ���� Node�� ���� �׷��Ƿ� 1������ �����Ѵ�.
	for (UINT i = 1; i < count; i++)
	{
		BoneInfo* pBone = skeleton->GetBone(i);
		assert(pBone != nullptr);
		assert(pBone->ParentBoneIndex != -1);

		Node* pParentNode = FindNode(skeleton->GetBoneName(pBone->ParentBoneIndex));
		assert(pParentNode != nullptr);
		
		auto& node = pParentNode->CreateChild();
		node.m_Name = pBone->Name;
		node.m_Local = pBone->RelativeTransform;
		node.m_Children.reserve(pBone->NumChildren);
		node.m_pParent = pParentNode;
		node.m_pAnimationTime = &m_AnimationProressTime;
	}
}

bool SkeletalMeshSceneResource::Create(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath.c_str());
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ ��� ��������


	unsigned int importFlags = aiProcess_Triangulate | // �ﰢ������ ��ȯ
		aiProcess_GenNormals |	// �븻 ����/
		aiProcess_GenUVCoords |		// UV ����
		aiProcess_CalcTangentSpace |  // ź��Ʈ ����
		aiProcess_LimitBoneWeights |	// ���� ������ �޴� ������ �ִ� ������ 4���� ����
		aiProcess_GenBoundingBoxes | // �ٿ�� �ڽ� ����
		aiProcess_ConvertToLeftHanded;	// �޼� ��ǥ��� ��ȯ

	const aiScene* scene = importer.ReadFile(filePath, importFlags);
	if (!scene) {
		LOG_ERRORA("Error loading file: %s", importer.GetErrorString());
		return false;
	}

	//������ Ȯ�ο� ���ӿ� �ʿ����
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

	// SceneResource�� �⺻ �ִϸ��̼� �߰��Ѵ�.

	assert(scene->mNumAnimations < 2); // �ִϸ��̼��� ���ų� 1�������Ѵ�. 
	// ����� �ִϸ��̼��� �ϳ��� ��ġ�� ����� FBX export���� NLA��Ʈ��,��� �׼� �ɼ��� ���ϴ�.
	if (scene->HasAnimations())
	{
		const aiAnimation* pAiAnimation = scene->mAnimations[0];
		// ä�μ��� aiAnimation �ȿ��� �ִϸ��̼� ������  ǥ���ϴ� aiNode�� �����̴�.
		assert(pAiAnimation->mNumChannels > 1); // �ִϸ��̼��� �ִٸ� aiNode �� �ϳ� �̻� �־���Ѵ�.
		
		Animation& ret = m_Animations.emplace_back();
		ret.Create(filePath, pAiAnimation);
	}
	importer.FreeScene();
	LOG_MESSAGEA("Complete file: %s", filePath.c_str());
	return true;
}


bool SkeletalMeshSceneResource::AddAnimation(std::string filePath)
{	
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath.c_str());
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ ��� ��������
	unsigned int importFlags = aiProcess_ConvertToLeftHanded;	// �޼� ��ǥ��� ��ȯ

	const aiScene* scene = importer.ReadFile(filePath, importFlags);
	if (!scene) {
		LOG_ERRORA("Error loading file: %s", importer.GetErrorString());
		return false;
	}

	assert(scene->mNumAnimations < 2); // �ִϸ��̼��� ���ų� 1�������Ѵ�. 
	// ����� �ִϸ��̼��� �ϳ��� ��ġ�� ����� FBX export���� NLA��Ʈ��,��� �׼� �ɼ��� ���ϴ�.
	if (scene->HasAnimations())
	{
		const aiAnimation* pAiAnimation = scene->mAnimations[0];
		// ä�μ��� aiAnimation �ȿ��� �ִϸ��̼� ������  ǥ���ϴ� aiNode�� �����̴�.
		assert(pAiAnimation->mNumChannels > 1); // �ִϸ��̼��� �ִٸ� aiNode �� �ϳ� �̻� �־���Ѵ�.

		Animation& ret = m_Animations.emplace_back();	// Vector�� �ı��� �����Ѵ�. ����ȭ�� ���߿�����
		ret.Create(filePath, pAiAnimation);
	}
	importer.FreeScene();
	LOG_MESSAGEA("Complete file: %s", filePath.c_str());
	return true;
}

Material* SkeletalMeshSceneResource::GetMeshMaterial(UINT index)
{
	assert(index < m_Materials.size());
	UINT mindex = m_SkeletalMeshResources[index].m_MaterialIndex;
	assert(mindex < m_Materials.size());
	return &m_Materials[mindex];
}

bool StaticMeshSceneResource::Create(std::string filePath)
{
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath.c_str());
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ ��� ��������

	unsigned int importFlags = aiProcess_Triangulate | // �ﰢ������ ��ȯ
		aiProcess_GenNormals |	// �븻 ����/
		aiProcess_GenUVCoords |		// UV ����
		aiProcess_CalcTangentSpace |  // ź��Ʈ ����	
		aiProcess_GenBoundingBoxes | // �ٿ�� �ڽ� ����
		aiProcess_ConvertToLeftHanded;	// �޼� ��ǥ��� ��ȯ

	const aiScene* scene = importer.ReadFile(filePath, importFlags);
	if (!scene) {
		LOG_ERRORA("Error loading file: %s", importer.GetErrorString());
		return false;
	}

	//������ Ȯ�ο� ���ӿ� �ʿ����
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
	LOG_MESSAGEA("Complete file: %s", filePath.c_str());
	return true;
}

Material* StaticMeshSceneResource::GetMeshMaterial(UINT index)
{
	assert(index < m_StaticMeshResources.size());
	UINT mindex = m_StaticMeshResources[index].m_MaterialIndex;
	assert(mindex < m_Materials.size());
	return &m_Materials[mindex];
}