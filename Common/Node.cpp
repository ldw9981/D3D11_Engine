#include "pch.h"
#include "Node.h"
#include "Model.h"
#include "Helper.h"
#include "SkeletonInfo.h"


Node::~Node()
{
	//LOG_MESSAGEA("~Node() %s", m_Name.c_str());	
}

void Node::CreateHierachy(SkeletonInfo* skeleton, float* pAnimationTime)
{
	UINT count = skeleton->GetBoneCount();

	BoneInfo* pBone = skeleton->GetBone(0);
	m_Name = pBone->Name;
	m_Children.reserve(pBone->NumChildren);	
		
	// 0�� ��Ʈ�� �����̳��̹Ƿ� ���� Node�� ���� �׷��Ƿ� 1������ �����Ѵ�.
	for (UINT i = 1; i < count; i++)
	{
		BoneInfo* pBone = skeleton->GetBone(i);
		assert(pBone != nullptr);
		assert(pBone->ParentBoneIndex != -1);

		Node* pParentNode = FindNode(skeleton->GetBoneName(pBone->ParentBoneIndex));
		assert(pParentNode != nullptr);

		auto& node = pParentNode->m_Children.emplace_back();
		node.m_Name = pBone->Name;
		node.m_Local = pBone->RelativeTransform;
		node.m_Children.reserve(pBone->NumChildren);
		node.m_pParent = pParentNode;
		node.m_pAnimationTime = pAnimationTime;
	}
}

void Node::Update(float DeltaTime)
{
	// ����� �ִϸ��̼��� �ִٸ� �ִϸ��̼��� ������Ʈ�Ѵ�.
	if (m_pNodeAnimation != nullptr)
	{
		Math::Vector3 position, scaling;
		Math::Quaternion rotation;
		m_pNodeAnimation->Evaluate(*m_pAnimationTime, position, rotation, scaling);
		m_Local = Math::Matrix::CreateScale(scaling) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
	}

	// �θ� ��尡 �ִٸ� �θ� ����� WorldMatrix�� ���ؼ� �ڽ��� WorldMatrix�� �����.
	if (m_pParent != nullptr)
		m_World = m_Local * m_pParent->m_World;
	else
		m_World = m_Local;

	for (auto& child : m_Children)
	{	// �ڽ� ������ Update()�� ȣ���Ѵ�.
		child.Update(DeltaTime);
	}
}

Node* Node::FindNode(const std::string& name)
{
	if (m_Name == name)
		return this;

	for (auto& child : m_Children)
	{
		Node* found = child.FindNode(name);
		if (found != nullptr)
			return found;
	}

	return nullptr;
}

