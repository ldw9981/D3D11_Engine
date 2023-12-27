#pragma once
#include "ActorController.h"
#include "InputManager.h"
/*
	�ֿ俪���� InputManager�� ����Ͽ� �Է� �̺�Ʈ�� �ް�
	PlayerController�� �������� Pawn�� �Է��� ó���ǵ����Ѵ�.
	���������� �ʴ� Pawn�� �Է��� ó������ �ʰ� �Ѵ�.
*/
class Pawn;
class PlayerController :
    public ActorController 
{
public:
	PlayerController();
	~PlayerController();

	virtual void OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker);

};

