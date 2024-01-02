#pragma once
#include "Transform.h"

struct aiNode;
class StaticMeshModel;
struct NodeAnimation;
struct SkeletonResource;
struct BoneInfo;


class Bone :public Transform
{
public:
	Bone() {};
	~Bone();

	std::string m_Name;		// Bone �̸�
	std::vector<Bone> m_Children;		// ���� ������ �̷�� �ڽ� ���
	NodeAnimation* m_pNodeAnimation = nullptr; // ��尡 ����� NodeAnimation�� �ִ� �ּ�
	float* m_pAnimationTime = nullptr;	// ���� ��尡 �ִϸ��̼ǿ��� ��� �ð��� �ִ���

	
	void SetAnimationTimePtr(float* val) { m_pAnimationTime = val; }
	virtual void Update(float DeltaTime);
	Bone* FindBone(const std::string& name);
	Bone& CreateChild();	
};
