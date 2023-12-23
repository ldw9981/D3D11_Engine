#include "pch.h"
#include "SceneComponent.h"


SceneComponent::SceneComponent(Actor* pOwner, const std::string& Name)
	:Component(pOwner,Name)
{	

}

SceneComponent::~SceneComponent()
{
}

void SceneComponent::Update(float DeltaTime)
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
