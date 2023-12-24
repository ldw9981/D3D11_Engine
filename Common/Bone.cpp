#include "pch.h"
#include "Bone.h"
#include "Helper.h"
#include "SkeletonResource.h"
#include "Animation.h"

Bone::~Bone()
{
	//LOG_MESSAGEA("~Node() %s", m_Name.c_str());	
}



void Bone::Update(float DeltaTime)
{
	// 노드의 애니메이션이 있다면 애니메이션을 업데이트한다.
	if (m_pNodeAnimation != nullptr)
	{
		Math::Vector3 position, scaling;
		Math::Quaternion rotation;
		m_pNodeAnimation->Evaluate(*m_pAnimationTime, position, rotation, scaling);
		m_Local = Math::Matrix::CreateScale(scaling) * Math::Matrix::CreateFromQuaternion(rotation) * Math::Matrix::CreateTranslation(position);
	}

	// 부모 노드가 있다면 부모 노드의 WorldMatrix를 곱해서 자신의 WorldMatrix를 만든다.
	if (m_pParent != nullptr)
		m_World = m_Local * m_pParent->m_World;
	else
		m_World = m_Local;

	for (auto& child : m_Children)
	{	// 자식 노드들의 Update()를 호출한다.
		child.Update(DeltaTime);
	}
}

Bone* Bone::FindNode(const std::string& name)
{
	if (m_Name == name)
		return this;

	for (auto& child : m_Children)
	{
		Bone* found = child.FindNode(name);
		if (found != nullptr)
			return found;
	}

	return nullptr;
}

Bone& Bone::CreateChild()
{
	return m_Children.emplace_back();	
}

