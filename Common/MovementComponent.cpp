#include "pch.h"
#include "MovementComponent.h"
#include "Actor.h"
#include "SceneComponent.h"

MovementComponent::MovementComponent()
{

}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::AddInputVector(const Math::Vector3& input)
{
	m_InputVector += input;
	m_InputVector.Normalize();		
}

void MovementComponent::Update(float DeltaTime)
{
	if (m_InputVector.Length() > 0.0f)
	{
		m_PositionBefore = m_pRootComponent->GetWorldPosition();
		m_PositionAfter = m_PositionBefore + m_InputVector * m_Speed * DeltaTime;
		m_pRootComponent->SetLocalPosition(m_PositionAfter);
		m_InputVector = Math::Vector3::Zero;
	}
}

void MovementComponent::OnBeginPlay()
{
	m_pRootComponent = GetOwner()->GetRootComponent();
}

void MovementComponent::OnEndPlay()
{

}


