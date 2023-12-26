#include "pch.h"
#include "ActorController.h"
#include "Pawn.h"

using namespace DirectX;

ActorController::ActorController()
{

}

ActorController::~ActorController()
{

}

void ActorController::AddPitch(float value)
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

void ActorController::AddYaw(float value)
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

void ActorController::Posess(Pawn* pPawn)
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

void ActorController::UnPosess()
{
	if (m_pPawn)
	{
		m_pPawn->OnUnPossess(this);
		m_pPawn = nullptr;
	}
}


