#pragma once
#include "Helper.h"

struct Bone
{
	Math::Matrix RelativeTransform;			//�θ�κ����� ������� ��ȯ
	Math::Matrix OffsetMatrix;				// �������� �޽��� ������� ��ġ(��ȯ)
	std::string Name;						// ���̸�
	int NumChildren=0;						// �ڽ��� ��
	int ParentBoneIndex=-1;
	std::vector<std::string> MeshNames;		// ���� ����� �޽����� �̸�
	Bone() {}
	void Set(const aiNode* pNode)
	{
		Name = std::string(pNode->mName.C_Str());
		RelativeTransform = Math::Matrix(&pNode->mTransformation.a1).Transpose();
		NumChildren = pNode->mNumChildren;
	}
	~Bone()
	{
		LOG_MESSAGEA("~Bone() %s", Name.c_str());
	}
};

// �� ������ �����ϴ� ����ü
struct Skeleton
{
public:
	std::string Name;
	std::vector<Bone> Bones;
	std::map<std::string,int> BoneMappingTable;
	std::map<std::string,int> MeshMappingTable;

	void Create(const aiScene* pScene);
	Bone* AddBone(const aiScene* pScene,const aiNode* pNode);
	Bone* FindBone(const std::string& name);
	Bone* GetBone(int index);
	int GetBoneIndexByBoneName(const std::string& boneName);	
	int GetBoneIndexByMeshName(const std::string& meshName);
	int GetBoneCount()  {  return (int)Bones.size(); }
	const std::string& GetBoneName(int index) { return Bones[index].Name; }
	void CountNode(int& Count,const aiNode* pNode);
};

