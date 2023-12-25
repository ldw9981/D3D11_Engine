#include "pch.h"
#include "SceneComponent.h"



SceneComponent::~SceneComponent()
{
}

void SceneComponent::SetLocalPosition(Math::Vector3 val)
{
	m_bDirty = true;
	m_LocalPosition = val;
}

void SceneComponent::SetLocalRotation(Math::Vector3 val)
{
	m_bDirty = true;
	m_LocalRotation = val;
}

void SceneComponent::SetLocalScale(Math::Vector3 val)
{
	m_bDirty = true;
	m_LocalScale = val;
}

void SceneComponent::Update(float DeltaTime)
{
	if (m_bDirty)
	{
		m_Local = Math::Matrix::CreateScale(m_LocalScale) * Math::Matrix::CreateFromYawPitchRoll(m_LocalRotation) * Math::Matrix::CreateTranslation(m_LocalPosition);
		m_bDirty = false;
	}	
	Transform::Update(DeltaTime);
}

void SceneComponent::OnBeginPlay()
{

}

void SceneComponent::OnEndPlay()
{

}

