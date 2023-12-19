#pragma once

struct aiNode;
class StaticMeshModel;
struct NodeAnimation;
struct SkeletonInfo;
struct BoneInfo;


class Node
{
public:
	Node() {};
	~Node();

	std::string m_Name;	
	Math::Matrix m_Local;	
	Math::Matrix m_World;
	std::vector<Node> m_Children;		// 계층 구조를 이루는 자식 노드
	Node* m_pParent = nullptr;
	NodeAnimation* m_pNodeAnimation = nullptr; // 노드가 사용할 NodeAnimation가 있는 주소
	float* m_pAnimationTime = nullptr;	// 현재 노드가 애니메이션에서 어느 시간에 있는지

	void CreateHierachy(SkeletonInfo* skeleton,float* pAnimationTime);

	void Update(float DeltaTime);
	Node* FindNode(const std::string& name);
};	

