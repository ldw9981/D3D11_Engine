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
		Math::Vector3 Position = m_pRootComponent->GetWorldPosition();
		Position += m_InputVector * m_Speed * DeltaTime;
		m_pRootComponent->SetRelativePosition(Position);
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


