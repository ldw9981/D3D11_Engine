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


void PlayerController::OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState)
{
	if (m_pPawn)
	{
		// Pawn��ü�� �Է�ó���� �Ҽ��������� Pawn�� PlayerController�� �������� �͸� �Է��� ó���ǵ����Ѵ�.
		m_pPawn->OnInputProcess(KeyboardState,MouseState);
	}
}