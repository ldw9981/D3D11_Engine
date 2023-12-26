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
	m_Pitch += value;
	if (m_Pitch > XM_PI)
	{
		m_Pitch -= XM_2PI;
	}
	else if (m_Pitch < -XM_PI)
	{
		m_Pitch += XM_2PI;
	}
}

void ActorController::AddYaw(float value)
{
	m_Yaw += value;
	if (m_Yaw > XM_PI)
	{
		m_Yaw -= XM_2PI;
	}
	else if (m_Yaw < -XM_PI)
	{
		m_Yaw += XM_2PI;
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


