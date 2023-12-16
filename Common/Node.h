#pragma once

struct aiNode;
class StaticMeshModel;
struct NodeAnimation;
struct Skeleton;
struct Bone;
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

	void CreateHierachy(StaticMeshModel* model,aiNode* node);
	void CreateHierachy(Skeleton* skeleton);

	void UpdateAnimation(float progressTime);
	Node* FindNode(const std::string& name);
};	

