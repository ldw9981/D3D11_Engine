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

	float m_Pitch = 0.0f;		// radian
	float m_Yaw = 0.0f;		// radian

	void AddPitch(float value);
	void AddYaw(float value);
	float GetYaw() { return m_Yaw; }
	float GetPitch() { return m_Pitch; }

	void Posess(Pawn* pPawn);
	void UnPosess();
};


