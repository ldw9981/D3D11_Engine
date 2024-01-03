#include "pch.h"
#include "Bone.h"
#include "Helper.h"
#include "SkeletonResource.h"
#include "AnimationResource.h"

Bone::~Bone()
{
	//LOG_MESSAGEA("~Node() %s", m_Name.c_str());	
}



void Bone::Update(float DeltaTime)
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
	__super::Update(DeltaTime);

	for (auto& child : m_Children)
	{	// �ڽ� ������ Update()�� ȣ���Ѵ�.
		child.Update(DeltaTime);
	}
}

Bone* Bone::FindBone(const std::string& name)
{
	if (m_Name == name)
		return this;

	for (auto& child : m_Children)
	{
		Bone* found = child.FindBone(name);
		if (found != nullptr)
			return found;
	}

	return nullptr;
}

Bone& Bone::CreateChild()
{
	return m_Children.emplace_back();	
}

