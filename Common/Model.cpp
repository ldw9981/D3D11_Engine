#include "pch.h"
#include "Model.h"
#include "Helper.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>
#include "ResourceManager.h"



bool StaticMeshModel::ReadFile(ID3D11Device* device,const char* filePath)
{
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath);
	Assimp::Importer importer;
	
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS,0);	// $assimp_fbx$ ��� ��������
	
	
	unsigned int importFlags = aiProcess_Triangulate | // �ﰢ������ ��ȯ
		aiProcess_GenNormals |	// �븻 ����/
		aiProcess_GenUVCoords |		// UV ����
		aiProcess_CalcTangentSpace |  // ź��Ʈ ����
		aiProcess_LimitBoneWeights |	// ���� ������ �޴� ������ �ִ� ������ 4���� ����
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
	
	
	m_pSkeleton = ResourceManager::Instance->CreateSkeleton(scene->mName.C_Str(),scene);
		
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		std::string key = path.string() + std::to_string(i);
		shared_ptr<Material> ret = ResourceManager::Instance->CreateMaterial(key, scene->mMaterials[i]);
		m_Materials.push_back(ret);
	}

	m_Meshes.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		m_Meshes[i].Create(device, scene->mMeshes[i], m_pSkeleton.get());
	}

	CreateHierachy(m_pSkeleton.get());
	//LoadSkeleton(this, scene->mRootNode);

	assert(scene->mNumAnimations < 2); // �ִϸ��̼��� ���ų� 1�������Ѵ�. 
	// ����� �ִϸ��̼��� �ϳ��� ��ġ�� ����� FBX export���� NLA��Ʈ��,��� �׼� �ɼ��� ���ϴ�.

	
	if (scene->HasAnimations())
	{
		const aiAnimation* pAiAnimation = scene->mAnimations[0];
		// ä�μ��� aiAnimation �ȿ��� �ִϸ��̼� ������  ǥ���ϴ� aiNode�� �����̴�.
		assert(pAiAnimation->mNumChannels > 1); // �ִϸ��̼��� �ִٸ� aiNode �� �ϳ� �̻� �־���Ѵ�.

		std::string key = m_pSkeleton->Name +"_" + std::string(filePath);
		shared_ptr<Animation> ret = ResourceManager::Instance->CreateAnimation(key, pAiAnimation);
		m_Animations.push_back(ret);
	
		// �� ���� �����ϴ� ���ִϸ��̼� ptr�� null�̹Ƿ� 0�� Index �ִϸ��̼��� ���ִϸ��̼��� �����Ѵ�.
		UpdateNodeAnimationReference(0);		
	}

	for (auto& mesh : m_Meshes)
	{
		mesh.UpdateNodeInstancePtr(this, m_pSkeleton.get());
	}


	importer.FreeScene();
	LOG_MESSAGEA("Complete file: %s", filePath);
	return true;
}

Material* StaticMeshModel::GetMaterial(UINT index)
{
	assert(index < m_Materials.size());
	return m_Materials[index].get();
}
void StaticMeshModel::Update(float deltaTime)
{
	if (!m_Animations.empty())
	{
		m_AnimationProressTime += deltaTime;
		m_AnimationProressTime = fmod(m_AnimationProressTime, m_Animations[0]->Duration);
		UpdateAnimation(m_AnimationProressTime);
	}	
}

void StaticMeshModel::UpdateNodeAnimationReference(UINT index)
{
	assert(index < m_Animations.size());
	Animation& animation = *m_Animations[index].get();
	for (size_t i = 0; i < animation.NodeAnimations.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation.NodeAnimations[i];
		Node* node = FindNode(nodeAnimation.NodeName);
		node->m_pNodeAnimation = &animation.NodeAnimations[i];
	}
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



bool SkeletalMeshModel::ReadFile(ID3D11Device* device, const char* filePath)
{
	std::filesystem::path path = ToWString(string(filePath));
	LOG_MESSAGEA("Loading file: %s", filePath);
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ ��� ��������


	unsigned int importFlags = aiProcess_Triangulate | // �ﰢ������ ��ȯ
		aiProcess_GenNormals |	// �븻 ����/
		aiProcess_GenUVCoords |		// UV ����
		aiProcess_CalcTangentSpace |  // ź��Ʈ ����
		aiProcess_LimitBoneWeights |	// ���� ������ �޴� ������ �ִ� ������ 4���� ����
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


	m_pSkeleton = ResourceManager::Instance->CreateSkeleton(scene->mName.C_Str(), scene);

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		std::string key = path.string() + std::to_string(i);
		shared_ptr<Material> ret = ResourceManager::Instance->CreateMaterial(key, scene->mMaterials[i]);
		m_Materials.push_back(ret);
	}

	m_Meshes.resize(scene->mNumMeshes);
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		m_Meshes[i].Create(device, scene->mMeshes[i], m_pSkeleton.get());
	}

	CreateHierachy(m_pSkeleton.get());
	//LoadSkeleton(this, scene->mRootNode);

	assert(scene->mNumAnimations < 2); // �ִϸ��̼��� ���ų� 1�������Ѵ�. 
	// ����� �ִϸ��̼��� �ϳ��� ��ġ�� ����� FBX export���� NLA��Ʈ��,��� �׼� �ɼ��� ���ϴ�.


	if (scene->HasAnimations())
	{
		const aiAnimation* pAiAnimation = scene->mAnimations[0];
		// ä�μ��� aiAnimation �ȿ��� �ִϸ��̼� ������  ǥ���ϴ� aiNode�� �����̴�.
		assert(pAiAnimation->mNumChannels > 1); // �ִϸ��̼��� �ִٸ� aiNode �� �ϳ� �̻� �־���Ѵ�.

		std::string key = m_pSkeleton->Name + "_" + std::string(filePath);
		shared_ptr<Animation> ret = ResourceManager::Instance->CreateAnimation(key, pAiAnimation);
		m_Animations.push_back(ret);

		// �� ���� �����ϴ� ���ִϸ��̼� ptr�� null�̹Ƿ� 0�� Index �ִϸ��̼��� ���ִϸ��̼��� �����Ѵ�.
		UpdateNodeAnimationReference(0);
	}

	for (auto& mesh : m_Meshes)
	{
		mesh.UpdateNodeInstancePtr(this, m_pSkeleton.get());
	}


	importer.FreeScene();
	LOG_MESSAGEA("Complete file: %s", filePath);
	return true;
}

Material* SkeletalMeshModel::GetMaterial(UINT index)
{
	assert(index < m_Materials.size());
	return m_Materials[index].get();
}
void SkeletalMeshModel::Update(float deltaTime)
{
	if (!m_Animations.empty())
	{
		m_AnimationProressTime += deltaTime;
		m_AnimationProressTime = fmod(m_AnimationProressTime, m_Animations[0]->Duration);
		UpdateAnimation(m_AnimationProressTime);
	}
}

void SkeletalMeshModel::UpdateNodeAnimationReference(UINT index)
{
	assert(index < m_Animations.size());
	Animation& animation = *m_Animations[index].get();
	for (size_t i = 0; i < animation.NodeAnimations.size(); i++)
	{
		NodeAnimation& nodeAnimation = animation.NodeAnimations[i];
		Node* node = FindNode(nodeAnimation.NodeName);
		node->m_pNodeAnimation = &animation.NodeAnimations[i];
	}
}

void SkeletalMeshModel::SetWorldTransform(const Math::Matrix& transform)
{
	m_Local = transform;
}