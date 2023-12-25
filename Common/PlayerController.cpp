#include "pch.h"
#include "PlayerController.h"
#include "Pawn.h"
using namespace DirectX;

void PlayerController::AddPitch(float value)
{
	m_MousePitch += value;
	if (m_MousePitch > XM_PI)
	{
		m_MousePitch -= XM_2PI;
	}
	else if (m_MousePitch < -XM_PI)
	{
		m_MousePitch += XM_2PI;
	}
}

void PlayerController::AddYaw(float value)
{
	m_MouseYaw += value;
	if (m_MouseYaw > XM_PI)
	{
		m_MouseYaw -= XM_2PI;
	}
	else if (m_MouseYaw < -XM_PI)
	{
		m_MouseYaw += XM_2PI;
	}
}

void PlayerController::Posess(Pawn* pPawn)
{	
	if (m_pPawn)
	{
		m_pPawn->OnUnPossess(this);
	}
	m_pPawn = pPawn;
	if (m_pPawn)
	{
		m_pPawn->OnPossess(this);
	}	
}

void PlayerController::UnPosess()
{
	if (m_pPawn)
	{
		m_pPawn->OnUnPossess(this);
		m_pPawn = nullptr;
	}
}

void PlayerController::OnBeginPlay()
{
	__super::OnBeginPlay();
	InputManager::Instance->AddInputProcesser(this);
	
}

void PlayerController::OnEndPlay()
{
	InputManager::Instance->RemoveInputProcesser(this);
	__super::OnEndPlay();
}

void PlayerController::Update(float DeltaTime)
{

}

void PlayerController::OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState)
{
	if (m_pPawn)
	{
		m_pPawn->OnInputProcess(KeyboardState,MouseState);
	}
}