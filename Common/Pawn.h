#pragma once
#include "Actor.h"
class PlayerController;
class Pawn :
    public Actor
{
public:
    Pawn();
    ~Pawn();

    PlayerController* m_pController = nullptr;
    PlayerController* GetController() { return m_pController; }

	virtual void OnPossess(PlayerController* pController);
	virtual void OnUnPossess(PlayerController* pController);
};

