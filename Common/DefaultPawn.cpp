#include "pch.h"
#include "DefaultPawn.h"

DefaultPawn::DefaultPawn()
{
	m_SceneComponent = CreateComponent<SceneComponent>("SceneComponent");
	SetRootComponent(m_SceneComponent);

	m_MovementComponent = CreateComponent<MovementComponent>("MovementComponent");
}

DefaultPawn::~DefaultPawn()
{
}

void DefaultPawn::OnBeginPlay()
{

}

void DefaultPawn::OnEndPlay()
{

}

void DefaultPawn::OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState)
{
	PlayerController* pController = GetController();

	if (KeyboardState.IsKeyDown(DirectX::Keyboard::Keys::W))
	{
		m_MovementComponent->AddInputVector(Math::Vector3(0.0f,0.0f,1.0f));
	}
	if (KeyboardState.IsKeyDown(DirectX::Keyboard::Keys::S))
	{
		m_MovementComponent->AddInputVector(Math::Vector3(0.0f, 0.0f,- 1.0f));
	}
}
