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

	virtual void OnInputProcess(const DirectX::Keyboard::State& KeyCurr, const DirectX::Keyboard::State& KeyLast,
		const DirectX::Mouse::State& MouseCurr, const DirectX::Mouse::State& MouseLast);

};

