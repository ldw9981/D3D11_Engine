#pragma once
#include "Actor.h"

class Pawn;
/*
	���� ȸ�����¸� �����ϴ� Actor 
	ĳ���Ϳ� ȸ�����¿� ����ȸ�����°� �ݵ�� ��ġ ������ �ʱ� ������ �����Ѵ�.
*/
class ActorController: public Actor
{
public:
	ActorController();
	~ActorController();

	Pawn* m_pPawn = nullptr;		// �����ϴ� ���

	float m_MousePitch = 0.0f;
	float m_MouseYaw = 0.0f;

	void AddPitch(float value);
	void AddYaw(float value);
	float GetYaw() { return m_MouseYaw; }
	float GetPitch() { return m_MousePitch; }

	void Posess(Pawn* pPawn);
	void UnPosess();
};


