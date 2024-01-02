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

	std::string m_Name;		// Bone 이름
	std::vector<Bone> m_Children;		// 계층 구조를 이루는 자식 노드
	NodeAnimation* m_pNodeAnimation = nullptr; // 노드가 사용할 NodeAnimation가 있는 주소
	float* m_pAnimationTime = nullptr;	// 현재 노드가 애니메이션에서 어느 시간에 있는지

	
	void SetAnimationTimePtr(float* val) { m_pAnimationTime = val; }
	virtual void Update(float DeltaTime);
	Bone* FindBone(const std::string& name);
	Bone& CreateChild();	
};
