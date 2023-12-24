#include "pch.h"
#include "Pawn.h"
#include "PlayerController.h"

Pawn::Pawn()
{

}

Pawn::~Pawn()
{

}

void Pawn::OnPossess(PlayerController* pController)
{
	m_pController = pController;
}

void Pawn::OnUnPossess(PlayerController* pController)
{
	m_pController = nullptr;
}
