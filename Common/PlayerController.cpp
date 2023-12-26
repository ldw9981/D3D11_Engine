#include "pch.h"
#include "PlayerController.h"
#include "Pawn.h"
using namespace DirectX;


PlayerController::PlayerController()
{
	b_UseInputProcesser = true;	// PlayerController는 InputManager에 등록하여 입력 이벤트를 받는다.
}

PlayerController::~PlayerController()
{

}


void PlayerController::OnInputProcess(const DirectX::Keyboard::State& KeyboardState, const DirectX::Mouse::State& MouseState)
{
	if (m_pPawn)
	{
		// Pawn자체가 입력처리를 할수도있지만 Pawn은 PlayerController가 제어중인 것만 입력이 처리되도록한다.
		m_pPawn->OnInputProcess(KeyboardState,MouseState);
	}
}