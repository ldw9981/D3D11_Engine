#pragma once
#include "ActorController.h"
#include "InputManager.h"
/*
	주요역할은 InputManager에 등록하여 입력 이벤트를 받고
	PlayerController가 제어중인 Pawn이 입력이 처리되도록한다.
	제어중이지 않는 Pawn은 입력이 처리되지 않게 한다.
*/
class Pawn;
class PlayerController :
    public ActorController 
{
public:
	PlayerController();
	~PlayerController();

	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState);
};

