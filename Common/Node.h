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
	std::vector<Node> m_Children;		// ���� ������ �̷�� �ڽ� ���
	Node* m_pParent = nullptr;
	NodeAnimation* m_pNodeAnimation = nullptr; // ��尡 ����� NodeAnimation�� �ִ� �ּ�
	float* m_pAnimationTime = nullptr;	// ���� ��尡 �ִϸ��̼ǿ��� ��� �ð��� �ִ���

	void CreateHierachy(SkeletonInfo* skeleton,float* pAnimationTime);

	void Update(float DeltaTime);
	Node* FindNode(const std::string& name);
};	

