#include "pch.h"
#include "SceneComponent.h"



SceneComponent::~SceneComponent()
{
}

void SceneComponent::SetRelativePosition(Math::Vector3 val)
{
	m_bDirty = true;
	m_RelativePosition = val;
}

void SceneComponent::SetRelativeRotation(Math::Vector3 val)
{
	m_bDirty = true;
	m_RelativeRotation = val;
}

void SceneComponent::SetRelativeScale(Math::Vector3 val)
{
	m_bDirty = true;
	m_RelativeScale = val;
}

void SceneComponent::Update(float DeltaTime)
{
	if (m_bDirty)
	{
		m_Local = Math::Matrix::CreateScale(m_RelativeScale) * Math::Matrix::CreateFromYawPitchRoll(m_RelativeRotation) * Math::Matrix::CreateTranslation(m_RelativePosition);
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

