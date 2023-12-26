#include "pch.h"
#include "Pawn.h"
#include "PlayerController.h"

Pawn::Pawn()
{

}

Pawn::~Pawn()
{

}

void Pawn::OnPossess(ActorController* pController)
{
	m_pController = pController;
}

void Pawn::OnUnPossess(ActorController* pController)
{
	m_pController = nullptr;
}
