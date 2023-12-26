#pragma once
#include "Actor.h"

class Pawn;
/*
	조작 회전상태를 저장하는 Actor 
	캐릭터와 회전상태와 조작회전상태가 반드시 일치 하지는 않기 때문에 구분한다.
*/
class ActorController: public Actor
{
public:
	ActorController();
	~ActorController();

	Pawn* m_pPawn = nullptr;		// 조작하는 대상

	float m_MousePitch = 0.0f;
	float m_MouseYaw = 0.0f;

	void AddPitch(float value);
	void AddYaw(float value);
	float GetYaw() { return m_MouseYaw; }
	float GetPitch() { return m_MousePitch; }

	void Posess(Pawn* pPawn);
	void UnPosess();
};


