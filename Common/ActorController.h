#pragma once
#include "Actor.h"

/*
	플레이어의 조작 회전상태를 저장하는 Actor 
	캐릭터와 회전상태와 조작회전상태가 반드시 일치 하지는 않기 때문에 구분한다.
	회전상태는 카메라의 회전이나 캐릭터의 이동방향을 결정하는데 사용한다.
*/
class Pawn;
class ActorController: public Actor
{
public:
	ActorController();
	~ActorController();

	Pawn* m_pPawn = nullptr;		// 조작하는 대상

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


