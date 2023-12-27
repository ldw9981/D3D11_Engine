#include "pch.h"
#include "PlayerController.h"
#include "Pawn.h"
using namespace DirectX;


PlayerController::PlayerController()
{
	b_UseInputProcesser = true;	// PlayerController�� InputManager�� ����Ͽ� �Է� �̺�Ʈ�� �޴´�.
}

PlayerController::~PlayerController()
{

}

void PlayerController::OnInputProcess(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker)
{
	if (m_pPawn)
	{
		// Pawn��ü�� �Է�ó���� �Ҽ��������� Pawn�� PlayerController�� �������� �͸� �Է��� ó���ǵ����Ѵ�.
		m_pPawn->OnInputProcess(KeyState, KeyTracker, MouseState, MouseTracker);
	}
}
