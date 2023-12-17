#include "pch.h"
#include "Animation.h"
#include "Helper.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#include <assimp/cimport.h>

void NodeAnimation::Create(aiNodeAnim* nodeAnimation,double tickPerSecond)
{
	assert(nodeAnimation != nullptr);
	assert(nodeAnimation->mNumPositionKeys == nodeAnimation->mNumRotationKeys);
	assert(nodeAnimation->mNumRotationKeys == nodeAnimation->mNumScalingKeys);
	
	NodeName = nodeAnimation->mNodeName.C_Str();
	LOG_MESSAGEA(NodeName.c_str());
	size_t numkeys = nodeAnimation->mNumPositionKeys;	
	AnimationKeys.resize(numkeys);
	
	for (size_t i = 0;i < numkeys;i++)
	{	
		aiVectorKey& pos = nodeAnimation->mPositionKeys[i];
		aiQuatKey& rot = nodeAnimation->mRotationKeys[i];
		aiVectorKey& scl = nodeAnimation->mScalingKeys[i];

		assert(pos.mTime == rot.mTime);
		assert(rot.mTime == scl.mTime);

		AnimationKeys[i].Time = (float)(pos.mTime / tickPerSecond);
		AnimationKeys[i].Position = { pos.mValue.x,pos.mValue.y,pos.mValue.z };
		AnimationKeys[i].Rotation = { rot.mValue.x,rot.mValue.y,rot.mValue.z, rot.mValue.w };
		AnimationKeys[i].Scaling =  { scl.mValue.x,scl.mValue.y,scl.mValue.z };
	}
}

void NodeAnimation::Evaluate(float time, Vector3& position, Quaternion& rotation, Vector3& scaling)
{
	assert(AnimationKeys.size() > 0);
	if (AnimationKeys.size() == 1)
	{
		position = AnimationKeys[0].Position;
		rotation = AnimationKeys[0].Rotation;
		scaling = AnimationKeys[0].Scaling;
	}
	else
	{
		size_t i = 0;
		for (i = 0; i < AnimationKeys.size(); i++)
		{
			if (AnimationKeys[i].Time > time)
			{
				break;
			}
		}

		if (i == 0)
		{
			position = AnimationKeys[i].Position;
			rotation = AnimationKeys[i].Rotation;
			scaling = AnimationKeys[i].Scaling;
		}
		else
		{
			float t = (time - AnimationKeys[i - 1].Time) / (AnimationKeys[i].Time - AnimationKeys[i - 1].Time);
			position = Vector3::Lerp(AnimationKeys[i - 1].Position, AnimationKeys[i].Position, t);
			rotation = Quaternion::Slerp(AnimationKeys[i - 1].Rotation, AnimationKeys[i].Rotation, t);
			scaling = Vector3::Lerp(AnimationKeys[i - 1].Scaling, AnimationKeys[i].Scaling, t);
		}
	}

}


void Animation::Create(const std::string strFBXFilePath,const aiAnimation* pAiAnimation)
{
	FilePath = strFBXFilePath;
	NodeAnimations.resize(pAiAnimation->mNumChannels);
	// ��ü �ð����� = �����Ӽ� / 1�ʴ� �����Ӽ�
	Duration = (float)(pAiAnimation->mDuration / pAiAnimation->mTicksPerSecond);
	for (size_t iChannel = 0; iChannel < pAiAnimation->mNumChannels; iChannel++)
	{
		aiNodeAnim* pAiNodeAnim = pAiAnimation->mChannels[iChannel];
		NodeAnimation& refNodeAnim = NodeAnimations[iChannel];
		refNodeAnim.Create(pAiNodeAnim, pAiAnimation->mTicksPerSecond);
	}
}

void Animation::Create(const std::string strFBXFilePath)
{
	std::filesystem::path path = ToWString(string(strFBXFilePath));
	LOG_MESSAGEA("Loading Animation FBX File: %s", strFBXFilePath.c_str());
	Assimp::Importer importer;

	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);	// $assimp_fbx$ ��� ��������
	unsigned int importFlags = aiProcess_ConvertToLeftHanded;	// �޼� ��ǥ��� ��ȯ
	const aiScene* scene = importer.ReadFile(strFBXFilePath, importFlags);
	if (!scene) {
		LOG_ERRORA("Error Loading Animation FBX File: %s", importer.GetErrorString());
		return;
	}
	assert(scene->mNumAnimations == 1); // �ִϸ��̼��� ���ų� 1�������Ѵ�. 
	// ����� �ִϸ��̼��� �ϳ��� ��ġ�� ����� FBX export���� NLA��Ʈ��,��� �׼� �ɼ��� ���ϴ�.

	const aiAnimation* pAiAnimation = scene->mAnimations[0];
		// ä�μ��� aiAnimation �ȿ��� �ִϸ��̼� ������  ǥ���ϴ� aiNode�� �����̴�.
	assert(pAiAnimation->mNumChannels > 1); // �ִϸ��̼��� �ִٸ� aiNode �� �ϳ� �̻� �־���Ѵ�.
		
	//shared_ptr<Animation> ret = m_Animations.emplace_back();
	//ret->Create(pAiAnimation);
	



	importer.FreeScene();
	LOG_MESSAGEA("Complete file: %s", strFBXFilePath.c_str());
}
