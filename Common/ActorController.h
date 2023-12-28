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

	float GetPitch() const { return m_Pitch; }
	void SetPitch(float val) { m_Pitch = val; }
	float GetYaw() const { return m_Yaw; }
	void SetYaw(float val) { m_Yaw = val; }
	void AddPitch(float value);
	void AddYaw(float value);

	void Posess(Pawn* pPawn);
	void UnPosess();
private:
	float m_Pitch = 0.0f;		// radian
	float m_Yaw = 0.0f;		// radian
};


