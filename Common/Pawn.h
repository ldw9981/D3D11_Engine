#pragma once
#include "Actor.h"
class ActorController;
class Pawn :
    public Actor
{
public:
    Pawn();
    ~Pawn();

    ActorController* m_pController = nullptr;
    ActorController* GetController() { return m_pController; }

	virtual void OnPossess(ActorController* pController);
	virtual void OnUnPossess(ActorController* pController);
};

