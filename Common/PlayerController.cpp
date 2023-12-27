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

void PlayerController::OnInputProcess(const DirectX::Keyboard::State& KeyCurr, const DirectX::Keyboard::State& KeyLast, const DirectX::Mouse::State& MouseCurr, const DirectX::Mouse::State& MouseLast)
{
	if (m_pPawn)
	{
		// Pawn��ü�� �Է�ó���� �Ҽ��������� Pawn�� PlayerController�� �������� �͸� �Է��� ó���ǵ����Ѵ�.
		m_pPawn->OnInputProcess(KeyCurr,KeyLast,MouseCurr, MouseLast);
	}
}
