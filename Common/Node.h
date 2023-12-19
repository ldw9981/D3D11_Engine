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
	std::vector<UINT> m_MeshIndices;	// �ش� ���� ����Ǵ� �޽� �ε���
	Node* m_pParent = nullptr;
	NodeAnimation* m_pNodeAnimation = nullptr; // ��尡 ����� NodeAnimation�� �ִ� �ּ�

	void CreateHierachy(SkeletonInfo* skeleton);

	void UpdateAnimation(float progressTime);
	void Update(float DeltaTime);
	Node* FindNode(const std::string& name);
};	

