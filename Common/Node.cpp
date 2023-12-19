#include "pch.h"
#include "Node.h"
#include "Model.h"
#include "Helper.h"
#include "SkeletonInfo.h"


Node::~Node()
{
	//LOG_MESSAGEA("~Node() %s", m_Name.c_str());	
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

