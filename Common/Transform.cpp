#include "pch.h"
#include "Transform.h"

void Transform::Update(float DeltaTime)
{
	if (m_pParent)
	{
		m_World = m_pParent->m_World * m_Local;
	}
	else
	{
		m_World = m_Local;
	}
}
