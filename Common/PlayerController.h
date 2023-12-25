#pragma once
#include "Actor.h"
#include "InputManager.h"

class Pawn;
class PlayerController :
    public Actor 
{
public:
	PlayerController() {};
	~PlayerController() {};

	Pawn* m_pPawn = nullptr;

	float m_MousePitch=0.0f;
	float m_MouseYaw=0.0f;

	void AddPitch(float value);
	void AddYaw(float value);
	float GetYaw() { return m_MouseYaw; }
	float GetPitch() { return m_MousePitch; }

	void Posess(Pawn* pPawn);
	void UnPosess();

	virtual void OnBeginPlay() override;
	virtual void OnEndPlay() override;
	virtual void Update(float DeltaTime) override;
	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState);
};

