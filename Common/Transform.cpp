#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
	m_Local = Math::Matrix::Identity;
	m_World = Math::Matrix::Identity;
	m_pParent = nullptr;
}

Transform::~Transform()
{
}

void Transform::Update(float DeltaTime)
{
	if (m_pParent)
	{
		m_World =  m_Local * m_pParent->m_World;
	}
	else
	{
		m_World = m_Local;
	}
}

Math::Vector3 Transform::GetWorldPosition()
{
	return m_World.Translation();
}

